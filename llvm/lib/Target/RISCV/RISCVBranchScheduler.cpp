//===-- RISCVBranchScheduler.cpp - RISC-V Branch Scheduler ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This pass hoists BMOVS_J/BMOVT_J (and their paired BMOVC_*/PB) as far up
// as possible. A "branch group" is the atomic unit {BMOVS, BMOVT, BMOVC, PB}
// that together represent one non-speculative branch. Because these
// instructions communicate through a shared branch register (b0, b1, ...),
// two groups cannot share a register if their live ranges would overlap
// after hoisting. Rather than track that positionally (which is fragile —
// see history below), we resolve conflicts up front by giving each group a
// register that's provably free among the groups hoisted so far, then move
// each group as a single, order-preserving atomic unit.
//
//===----------------------------------------------------------------------===//

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
#include "RISCVMachineFunctionInfo.h"
#include "RISCVNonSpec.h"
#include "RISCVSubtarget.h"
#include "RISCVTargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "riscv-branch-scheduler"
#define RISCV_BRANCH_SCHEDULER_PASS_NAME "RISC-V Branch Scheduler"

//===----------------------------------------------------------------------===//
// BranchGroupInfo — discovers {BMOVS, BMOVT, BMOVC, PB} groups within a
// single basic block and reassigns branch registers to eliminate conflicts.
//===----------------------------------------------------------------------===//

namespace {

class BranchGroupInfo {
public:
  struct Group {
    MachineInstr *BMOVS = nullptr;
    MachineInstr *BMOVT = nullptr;
    MachineInstr *BMOVC = nullptr; // optional: unconditional jumps have none
    MachineInstr *PB = nullptr;
  };

  // Scans one basic block for branch groups, in program order, and
  // rewrites their branch-register operands in place so that no two
  // groups within this block ever collide. Must be called once per MBB
  // before hoisting.
  void build(MachineBasicBlock &MBB) {
    Groups.clear();
    UsedBRs.clear();

    for (MachineInstr &MI : MBB) {
      if (MI.getOpcode() != RISCV::BMOVS_J)
        continue;

      Group G;
      G.BMOVS = &MI;
      MCRegister OldReg = MI.getOperand(0).getReg().asMCReg();

      // Find the matching BMOVT_J (same register, later in the block).
      auto It = MI.getIterator();
      for (++It; It != MBB.end(); ++It) {
        if (It->getOpcode() == RISCV::BMOVT_J || It->getOpcode() == RISCV::BMOVT_I && hasReg(*It, OldReg)) {
          G.BMOVT = &*It;
          break;
        }
      }

      // From the BMOVT, find the BMOVC (if any — conditional branches only)
      // and the PB that consumes this register.
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

      // Conflict resolution: if this register is already claimed by an
      // earlier group in this block, reassign this group to a free
      // physical branch register and rewrite every member instruction.
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
          // Out of branch registers: leave this group on OldReg. It will
          // not be safe to hoist past the group that currently owns
          // OldReg, so skip recording it — it stays where it is.
          LLVM_DEBUG(dbgs() << "BR pool exhausted in " << printMBBReference(MBB)
                            << "; leaving group in place\n");
          continue;
        }

        UsedBRs.insert(NewReg);
        for (MachineInstr *Member : {G.BMOVS, G.BMOVT, G.BMOVC, G.PB}) {
          if (!Member)
            continue;
          for (MachineOperand &MO : Member->operands())
            if (MO.isReg() && MO.getReg() == OldReg)
              MO.setReg(NewReg);
        }
      } else {
        UsedBRs.insert(OldReg);
      }

      Groups.push_back(G);
    }
  }

  MutableArrayRef<Group> groups() { return Groups; }

void print(raw_ostream &OS, const TargetRegisterInfo *TRI = nullptr) const {
  int GroupIdx = 0;
  for (const auto &G : Groups) {
    OS << "Group " << GroupIdx++ << ": ";
    // Print the branch register (taken from BMOVS, which always has it at operand 0)
    if (G.BMOVS) {
      Register Reg = G.BMOVS->getOperand(0).getReg();
      if (TRI)
        OS << printReg(Reg, TRI);
      else
        OS << Reg;
    } else {
      OS << "<no BMOVS>";
    }
    OS << " members: ";
    if (G.BMOVS) OS << "BMOVS ";
    if (G.BMOVT) OS << "BMOVT ";
    if (G.BMOVC) OS << "BMOVC ";
    if (G.PB)    OS << "PB ";
    OS << "\n";
  }
}

void dump() const {
  print(dbgs());
} 

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
      if (MO.isReg() && MO.getReg().isValid() && MO.getReg().asMCReg() == Reg)
        return true;
    return false;
  }

  SmallVector<Group, 16> Groups;
  SmallSet<MCRegister, 32> UsedBRs;
};

//===----------------------------------------------------------------------===//
// RISCVBranchScheduler
//===----------------------------------------------------------------------===//

class RISCVBranchScheduler : public MachineFunctionPass {
public:
  static char ID;

  RISCVBranchScheduler() : MachineFunctionPass(ID) {
    initializeRISCVBranchSchedulerPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return RISCV_BRANCH_SCHEDULER_PASS_NAME;
  }
};

char RISCVBranchScheduler::ID = 0;

bool RISCVBranchScheduler::runOnMachineFunction(MachineFunction &MF) {
  bool Changed = false;
  int TotalGroups = 0;
  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();


  
  dbgs() << "---- FUNC"<<MF.getName()<<"-----" << "\n";
  for (MachineBasicBlock &MBB : MF) {
    BranchGroupInfo BGI;          // Local per block
    BGI.build(MBB);               // Discovers groups and reassigns registers

    // Print groups if requested (before hoisting)
    if (1) {
      dbgs() << "Groups in " << printMBBReference(MBB) << ":\n";
      BGI.print(dbgs(), TRI);
    }
    LLVM_DEBUG({
      dbgs() << "Groups in " << printMBBReference(MBB) << ":\n";
      BGI.print(dbgs(), TRI);
    });

    // Hoist BMOVS and BMOVT to the top of the block
    MachineBasicBlock::iterator InsertPt = MBB.begin();
    for (auto &G : BGI.groups()) {
      // Move BMOVS
      if (G.BMOVS) {
        if (G.BMOVS->getIterator() != InsertPt) {
          MBB.splice(InsertPt, &MBB, G.BMOVS->getIterator());
          Changed = true;
        }
        InsertPt = std::next(G.BMOVS->getIterator());
      }
      // Move BMOVT
      if (G.BMOVT) {
        if (G.BMOVT->getIterator() != InsertPt) {
          MBB.splice(InsertPt, &MBB, G.BMOVT->getIterator());
          Changed = true;
        }
        InsertPt = std::next(G.BMOVT->getIterator());
      }
      ++TotalGroups;
    }
  }

  LLVM_DEBUG(dbgs() << "Ran " RISCV_BRANCH_SCHEDULER_PASS_NAME " on "
                    << MF.getName() << " (" << TotalGroups << " groups)\n");
  return Changed;
}

} // end of anonymous namespace

INITIALIZE_PASS(RISCVBranchScheduler, "riscv-branch-scheduler",
                RISCV_BRANCH_SCHEDULER_PASS_NAME,
                false, // is CFG only?
                false  // is analysis?
)

namespace llvm {

FunctionPass *createRISCVBranchSchedulerPass() {
  return new RISCVBranchScheduler();
}

} // namespace llvm