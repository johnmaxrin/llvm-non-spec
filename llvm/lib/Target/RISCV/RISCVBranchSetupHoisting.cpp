//==- RISCVBranchSetupHoisting.cpp - Hoist BMOV insts. towards fn. entry -====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "RISCVBranchSetupAnalysis.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineLoopInfo.h"

using namespace llvm;

#define RISCV_BRANCH_SETUP_HOISTING_PASS_NAME                                  \
  "RISC-V branch setup hoisting pass"

namespace {

class RISCVBranchSetupHoisting : public MachineFunctionPass {
public:
  static char ID;
  const RISCVBranchSetupInfo *BSI;
  const TargetInstrInfo *TII;
  const TargetRegisterInfo *TRI;
  MachineDominatorTree *MDT;
  MachineLoopInfo *MLI;

  RISCVBranchSetupHoisting() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    MachineFunctionPass::getAnalysisUsage(AU);
    AU.addRequired<RISCVBranchSetupAnalysisWrapper>();
    AU.addRequired<MachineDominatorTreeWrapperPass>();
    AU.addRequired<MachineLoopInfoWrapperPass>();
    AU.setPreservesAll();
  }

  StringRef getPassName() const override {
    return RISCV_BRANCH_SETUP_HOISTING_PASS_NAME;
  }

  bool scheduleBranchSetup(MachineInstr &MI, MachineInstr *S, MachineInstr *T,
                           MachineInstr *C);
  MachineBasicBlock::iterator findEarliestSafePoint(MachineInstr *SetupMI,
                                                    MachineInstr &BranchMI);
};

char RISCVBranchSetupHoisting::ID = 0;

bool RISCVBranchSetupHoisting::runOnMachineFunction(MachineFunction &MF) {
  TII = MF.getSubtarget().getInstrInfo();
  TRI = MF.getSubtarget().getRegisterInfo();
  BSI = &getAnalysis<RISCVBranchSetupAnalysisWrapper>().getInfo();
  MDT = &getAnalysis<MachineDominatorTreeWrapperPass>().getDomTree();
  MLI = &getAnalysis<MachineLoopInfoWrapperPass>().getLI();

  if (BSI->Branches.size() > 30)
    return false;

  // Collect all (BranchMI, BranchSetup) pairs across ALL blocks FIRST.
  struct BranchWork {
    MachineInstr *MI;
    MachineInstr *S, *T, *C;
  };
  SmallVector<BranchWork, 16> WorkList;

  for (auto &MBB : MF) {
    for (auto &MI : MBB) {
      if (RISCVBranchSetup BS = BSI->Branches.lookup(&MI)) {
        WorkList.push_back({
          &MI,
          const_cast<MachineInstr *>(BS.S),
          const_cast<MachineInstr *>(BS.T),
          const_cast<MachineInstr *>(BS.C)
        });
      }
    }
  }

 
  bool Changed = false;
  for (auto &W : WorkList)
    Changed |= scheduleBranchSetup(*W.MI, W.S, W.T, W.C);

  return Changed;
}

} // end of anonymous namespace

bool RISCVBranchSetupHoisting::scheduleBranchSetup(MachineInstr &MI,
                                                   MachineInstr *S,
                                                   MachineInstr *T,
                                                   MachineInstr *C) {
  MachineBasicBlock::iterator InsertPt;
  assert(S && "Branch must have BMOVS");
  assert(T && "Branch must have BMOVT");
  bool Changed = false;

  // dbgs() << "Setup for:"; MI.dump();

  InsertPt = findEarliestSafePoint(S, MI);
  if (InsertPt != S->getIterator()) {
    InsertPt->getParent()->splice(InsertPt, S->getParent(), S);
    Changed = true;
  }
  InsertPt = findEarliestSafePoint(T, MI);
  if (InsertPt != T->getIterator()) {
    InsertPt->getParent()->splice(InsertPt, T->getParent(), T);
    Changed = true;
  }
  if (C) {
    InsertPt = findEarliestSafePoint(C, MI);
    if (InsertPt != C->getIterator()) {
      InsertPt->getParent()->splice(InsertPt, C->getParent(), C);
      Changed = true;
    }
  }
  return Changed;
}

static bool redefinesSourceRegs(const MachineInstr &MI,
                                const MachineInstr &SetupMI,
                                const TargetRegisterInfo *TRI) {
  for (const MachineOperand &MO : SetupMI.explicit_uses()) {
    if (MO.isReg() && MI.modifiesRegister(MO.getReg(), TRI))
      return true;
  }
  return false;
}

static bool isClobberedByCall(const MachineInstr &CallMI,
                              const MachineInstr &SetupMI) {
  for (const MachineOperand &MO : CallMI.operands()) {
    if (MO.isRegMask()) {
      // Check if b0 or any source reg in SetupMI is clobbered by call mask
      for (const MachineOperand &Use : SetupMI.operands()) {
        if (Use.isReg() && MO.clobbersPhysReg(Use.getReg()))
          return true;
      }
    }
  }
  return false;
}

/// Scan `BB` backwards from `From`, returning the earliest safe
/// insertion point. `HazardFound` is set if we stopped early.
/// Caller guarantees From != BB->rend().
static MachineBasicBlock::iterator
scanBlockBackward(MachineBasicBlock *BB,
                  MachineBasicBlock::reverse_iterator From,
                  MachineInstr *SetupMI,
                  const TargetRegisterInfo *TRI,
                  bool &HazardFound) {
  HazardFound = false;
  
  // Try to hoist to the top of the func.
  MachineBasicBlock::iterator SafePoint = BB->getFirstNonPHI();

  for (auto I = From, E = BB->rend(); I != E; ++I) {
    MachineInstr &CurrMI = *I;

    
    if (CurrMI.isPHI())
      break;

    if (redefinesSourceRegs(CurrMI, *SetupMI, TRI)) {
      dbgs() << "  [Hazard] Source reg redefined by: ";
      CurrMI.dump();
      HazardFound = true;

      // Insert after this instr. 
      return std::next(CurrMI.getIterator());
    }

    if (CurrMI.isCall()) {
      dbgs() << "  [Hazard] Call boundary at: ";
      CurrMI.dump();
      HazardFound = true;
      return std::next(CurrMI.getIterator());
    }

    SafePoint = CurrMI.getIterator();
  }

  return SafePoint;
}

MachineBasicBlock::iterator RISCVBranchSetupHoisting::findEarliestSafePoint(
    MachineInstr *SetupMI, MachineInstr &BranchMI) {

  MachineBasicBlock *CurBB = SetupMI->getParent();
  MachineBasicBlock::iterator SafePoint = SetupMI->getIterator();

  // ---- Intra-block scan ---- Same as before. 
  bool HazardFound = false;

  auto IntraStart = std::next(SetupMI->getReverseIterator());
  if (IntraStart != CurBB->rend())
    SafePoint = scanBlockBackward(CurBB, IntraStart, SetupMI, TRI, HazardFound);
  else
    SafePoint = CurBB->getFirstNonPHI(); // SetupMI is already at the top.

  if (HazardFound)
    return SafePoint;

  
  //----- Cross-BB hoisting via the IDom chain ---- 
  MachineLoop *SetupLoop = MLI->getLoopFor(CurBB);
  MachineBasicBlock *BB = CurBB;

  while (true) {
    MachineDomTreeNode *Node = MDT->getNode(BB);
    if (!Node)
      break;

    MachineDomTreeNode *IDomNode = Node->getIDom();
    if (!IDomNode)
      break;

    MachineBasicBlock *IDom = IDomNode->getBlock();
    if (!IDom || IDom->empty())
      break;

    // Don't hoist across loop boundaries.
    MachineLoop *IDomLoop = MLI->getLoopFor(IDom);
    if (IDomLoop != SetupLoop) {
      dbgs() << "  [Stop] Loop boundary at BB#" << IDom->getNumber() << "\n";
      break;
    }

    // Find scan start: just before the first terminator.
    // Walk rbegin() forward past all terminators.
    MachineBasicBlock::reverse_iterator IDomScanStart = IDom->rbegin();
    while (IDomScanStart != IDom->rend() && IDomScanStart->isTerminator())
      ++IDomScanStart;

    // FIX: correct degenerate check — rend() means no non-terminator instrs.
    if (IDomScanStart == IDom->rend()) {
      dbgs() << "  [Skip] IDom BB#" << IDom->getNumber()
             << " has no non-terminator instructions\n";
      // Can still try to insert at block top (before terminators).
      SafePoint = IDom->getFirstTerminator();
      BB = IDom;
      continue; // Try climbing higher.
    }

    MachineBasicBlock::iterator IDomSafePoint =
        scanBlockBackward(IDom, IDomScanStart, SetupMI, TRI, HazardFound);

    dbgs() << "  [Cross-BB] Hoisted into BB#" << IDom->getNumber() << "\n";
    SafePoint = IDomSafePoint;
    BB = IDom;

    if (HazardFound)
      break;
  }

  return SafePoint;
}



INITIALIZE_PASS(RISCVBranchSetupHoisting, "riscv-branch-setup-hoisting",
                RISCV_BRANCH_SETUP_HOISTING_PASS_NAME,
                false, // is CFG only?
                false  // is analysis?
)

namespace llvm {

FunctionPass *createRISCVBranchSetupHoistingPass() {
  return new RISCVBranchSetupHoisting();
}

} // namespace llvm
