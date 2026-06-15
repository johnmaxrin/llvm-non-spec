//===-- RISCVBranchScheduler.cpp - RISC-V Branch Scheduler ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/PBQP/Math.h"

using namespace llvm;

#define RISCV_BRANCH_SCHEDULER_PASS_NAME "RISC-V Branch Scheduler"

namespace {

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

struct Branch {
  MachineInstr *BMOVS;
  MachineInstr *BMOVT;
  MachineInstr *BMOVC;
  MachineInstr *PB;

  explicit Branch(MachineInstr *BS, MachineInstr *BT, MachineInstr *PB)
  : BMOVS(BS), BMOVT(BT), PB(PB) {}

  Branch parseFromPB(MachineInstr* PB) {

  }
};

bool RISCVBranchScheduler::runOnMachineFunction(MachineFunction &MF) {
  int Count = 0;

  MF.dump();

  // - Keep track of how many branch registers we need
  // - > Register spilling requires extra bmov's
  // - > Register allocator would be helpful here
  // - > BMOVS, BMOVT should be moved as far up as possible
  // - > BMOVC can only bemov'd after it's register's defs
  // - > > Need way to distinguish BMOVT,BMOVS from BMOVC_CC
  // - > Function calls act as hard barriers (for now)
  // - > > In the future we can maybe add BMOV information to function signatures
  // - Need to separate CALL, RET, INDIRECT

  int BranchRegistersNeeded = 0;

  for (auto &MBB : MF) {
    for (auto I = MBB.rbegin(), E = MBB.rend(); I != E; ) {
      MachineInstr &MI = *I;
      ++I; // Increment the iterator BEFORE moving the instruction

      if (MI.isBMOV()) {
        // MI.removeFromParent();
        // MBB.insert(MBB.getFirstNonPHI(), &MI);
        Count += 1;
        // dbgs() << "Moved ";
        // MI.dump();
      }
    }
  }
  // MF.dump();
  dbgs() << "Ran " RISCV_BRANCH_SCHEDULER_PASS_NAME " on " << MF.getName() << " (" << Count << ")\n";
  return false;
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
