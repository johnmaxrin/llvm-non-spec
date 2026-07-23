#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include <algorithm>

#include "MCTargetDesc/RISCVMCTargetDesc.h"
#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "RISCVNonSpec.h"
#include "RISCVSubtarget.h"
#include "RISCVTargetMachine.h"
#include "RISCVMachineFunctionInfo.h"

using namespace llvm;

#define DEBUG_TYPE "riscv-ns-branch-opt"
#define PASS_NAME "riscv-ns-branch-opt"

class BranchGroupInfo {
public:
  struct Group {
    MachineInstr *BMOVS = nullptr;
    MachineInstr *BMOVT = nullptr;
    MachineInstr *BMOVC = nullptr;
    MachineInstr *PB    = nullptr;
    MachineBasicBlock *MBB = nullptr;
  };

  // Builds groups AND assigns/rewrites fresh branch registers
  // as conflicts are detected.
  void build(MachineFunction &MF) {
    Groups.clear();
    InstrToGroupIdx.clear();
    UsedBRs.clear();

    for (MachineBasicBlock &MBB : MF) {
      for (MachineInstr &MI : MBB) {
        if (MI.getOpcode() != RISCV::BMOVS_J)
          continue;

        Group G;
        G.MBB  = &MBB;
        G.BMOVS = &MI;

        MCRegister OldReg = MI.getOperand(0).getReg().asMCReg();

        // --- Find BMOVT_J with the same register ---
        auto It = MI.getIterator();
        for (++It; It != MBB.end(); ++It) {
          if (It->getOpcode() == RISCV::BMOVT_J && hasReg(*It, OldReg)) {
            G.BMOVT = &*It;
            break;
          }
        }

        // --- From there, find BMOVC and PB with the same register ---
        if (G.BMOVT) {
          auto It2 = G.BMOVT->getIterator();
          for (++It2; It2 != MBB.end(); ++It2) {
            if (!G.BMOVC && isBMOVC(It2->getOpcode()) && hasReg(*It2, OldReg)) {
              G.BMOVC = &*It2;
              continue;
            }
            if (!G.PB && RISCVNS::isPB(It2->getOpcode()) &&
                hasReg(*It2, OldReg)) {
              G.PB = &*It2;
              break;
            }
          }
        }

        // --- Register reassignment for this group ---
        if (UsedBRs.count(OldReg)) {
          MCRegister NewReg;
          bool Found = false;
          for (MCPhysReg Candidate : RISCV::PBRRegClass) {
            if (!UsedBRs.count(Candidate)) {
              NewReg = Candidate;
              Found = true;
              break;
            }
          }

          if (!Found) {
            LLVM_DEBUG(dbgs() << "BR pool exhausted for group in "
                              << printMBBReference(MBB) << "\n");
            // Leave OldReg as-is; group is still recorded below.
          } else {
            UsedBRs.insert(NewReg);
            for (MachineInstr *Member : {G.BMOVS, G.BMOVT, G.BMOVC, G.PB}) {
              if (!Member) continue;
              for (MachineOperand &MO : Member->operands())
                if (MO.isReg() && MO.getReg() == OldReg)
                  MO.setReg(NewReg);
            }
          }
        } else {
          UsedBRs.insert(OldReg);
        }

        // --- Record the group ---
        unsigned Idx = Groups.size();
        Groups.push_back(G);
        Group &Stored = Groups.back();
        if (Stored.BMOVS) InstrToGroupIdx[Stored.BMOVS] = Idx;
        if (Stored.BMOVT) InstrToGroupIdx[Stored.BMOVT] = Idx;
        if (Stored.BMOVC) InstrToGroupIdx[Stored.BMOVC] = Idx;
        if (Stored.PB)    InstrToGroupIdx[Stored.PB]    = Idx;
      }
    }
  }

  Group *getGroup(MachineInstr *MI) {
    auto It = InstrToGroupIdx.find(MI);
    if (It == InstrToGroupIdx.end())
      return nullptr;
    return &Groups[It->second];
  }

  MachineInstr *getBMOVS(MachineInstr *MI) { auto *G = getGroup(MI); return G ? G->BMOVS : nullptr; }
  MachineInstr *getBMOVT(MachineInstr *MI) { auto *G = getGroup(MI); return G ? G->BMOVT : nullptr; }
  MachineInstr *getBMOVC(MachineInstr *MI) { auto *G = getGroup(MI); return G ? G->BMOVC : nullptr; }
  MachineInstr *getPB(MachineInstr *MI)    { auto *G = getGroup(MI); return G ? G->PB    : nullptr; }

  MutableArrayRef<Group> groups() { return Groups; }

private:
  static bool isBMOVC(unsigned Opc) {
    switch (Opc) {
    case RISCV::BMOVC_BNE:
    case RISCV::BMOVC_BEQ:
    case RISCV::BMOVC_BGE:
    case RISCV::BMOVC_BGEU:
    case RISCV::BMOVC_BLT:
    case RISCV::BMOVC_BLTU:
    case RISCV::BMOVC_BITS:
    case RISCV::BMOVC_LOOP:
      return true;
    default:
      return false;
    }
  }

  static bool hasReg(MachineInstr &MI, MCRegister Reg) {
    for (MachineOperand &MO : MI.operands())
      if (MO.isReg() && MO.getReg().isValid() &&
          MO.getReg().asMCReg() == Reg)
        return true;
    return false;
  }

  SmallVector<Group, 16> Groups;
  DenseMap<MachineInstr *, unsigned> InstrToGroupIdx;
  SmallSet<MCRegister, 32> UsedBRs;
};


namespace {
class RISCVNSBranchOpt : public MachineFunctionPass {

public:
  static char ID;
  RISCVNSBranchOpt() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
  BranchGroupInfo BGI;
  BGI.build(MF);   // builds groups AND renames registers in-place

  MachineBasicBlock &EntryBlk = MF.front();
  MachineBasicBlock::iterator InsertionPt = EntryBlk.getFirstNonPHI();

  bool Changed = false;

  // Build BMOVSMap / BMOVTMap from the groups
  DenseMap<MachineInstr*, MachineInstr*> BMOVSMap;
  DenseMap<MachineInstr*, MachineInstr*> BMOVTMap;

  for (auto &G : BGI.groups()) {
    if (G.PB && G.BMOVS) BMOVSMap[G.PB] = G.BMOVS;
    if (G.PB && G.BMOVT) BMOVTMap[G.BMOVT] = G.PB;

    // Hoist BMOVS_J / BMOVT_J to entry, now correctly renamed
    if (G.BMOVS) {
      EntryBlk.splice(InsertionPt, G.MBB, G.BMOVS->getIterator());
      Changed = true;
    }
    if (G.BMOVT) {
      EntryBlk.splice(InsertionPt, G.MBB, G.BMOVT->getIterator());
      Changed = true;
    }
  }

  // MF.getInfo<RISCVMachineFunctionInfo>()->setBMOVSMap(std::move(BMOVSMap));
  // MF.getInfo<RISCVMachineFunctionInfo>()->setBMOVTMap(std::move(BMOVTMap));

  return Changed;
}

  StringRef getPassName() const override {
    return "RISCV NS Branch Optimization";
  }
  
};
} // end anonymous namespace

char RISCVNSBranchOpt::ID = 0;

INITIALIZE_PASS(RISCVNSBranchOpt, PASS_NAME,
                "RISCV Non Speculative Branch Optimization", false, true)

FunctionPass *llvm::createRISCVNSBranchOptPass() {
  return new RISCVNSBranchOpt();
}