//==- RISCVExpandBranchPseudoInsts.cpp - Expand branch pseudo instrs. -----===//
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

using namespace llvm;

#define RISCV_EXPAND_BRANCH_PSEUDO_PASS_NAME                                   \
  "RISC-V branch pseudo instruction expansion pass"

namespace {

class RISCVExpandBranchPseudo : public MachineFunctionPass {
public:
  static char ID;

  RISCVExpandBranchPseudo() : MachineFunctionPass(ID) {
    initializeRISCVExpandBranchPseudoPass(*PassRegistry::getPassRegistry());
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override {
    return RISCV_EXPAND_BRANCH_PSEUDO_PASS_NAME;
  }
};

char RISCVExpandBranchPseudo::ID = 0;

bool RISCVExpandBranchPseudo::runOnMachineFunction(MachineFunction &MF) {
  (void)(MF);
  //dbgs() << "Ran " RISCV_EXPAND_BRANCH_PSEUDO_PASS_NAME " on "
  //  << MF.getName() << "\n";
  return false;
}

} // end of anonymous namespace

INITIALIZE_PASS(RISCVExpandBranchPseudo, "riscv-expand-branch-pseudo",
                RISCV_EXPAND_BRANCH_PSEUDO_PASS_NAME,
                false, // is CFG only?
                false  // is analysis?
)

namespace llvm {

FunctionPass *createRISCVExpandBranchPseudoPass() {
  return new RISCVExpandBranchPseudo();
}

} // namespace llvm
