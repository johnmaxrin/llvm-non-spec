//===-- RISCVBranchSupportAnalysis.cpp - Branch support analysis ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM
// Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares RISCVBranchSupportAnalysis, a read-only MachineFunction
// analysis meant to run at the very end of the RISC-V backend pipeline,
// after both RISCVExpandPseudoInsts and RISCVExpandAtomicPseudoInsts have
// run. At that point every remaining MachineInstr should be a "real" (i.e.
// non-pseudo) target instruction -- exactly the instruction stream that is
// about to be handed to the AsmPrinter/MC layer.
//
// The analysis never mutates the MachineFunction (AU.setPreservesAll()), so
// it can be inserted anywhere late in the pipeline without perturbing
// codegen.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_RISCVBRANCHSUPPORTANALYSIS_H
#define LLVM_LIB_TARGET_RISCV_RISCVBRANCHSUPPORTANALYSIS_H

#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachinePassManager.h"

namespace llvm {

class MachineFunction;
class Module;
class raw_ostream;

struct RISCVBranchSupport {
  const MachineInstr *S; // Source
  const MachineInstr *T; // Target
  const MachineInstr *C; // Condition (optional)

  operator bool() const { return S || T || C; }
  void dump() const;
  void print(raw_ostream &OS) const;
};

struct RISCVBranchSupportInfo {
  unsigned NumBMOVS = 0;
  unsigned NumBMOVT = 0;
  unsigned NumBMOVC = 0;
  unsigned NumPB = 0;

  DenseMap<const MachineInstr*, RISCVBranchSupport> Branches;

  void print(raw_ostream &OS, const MachineFunction &MF) const;
};

/// Legacy PassManager wrapper. Other legacy-PM passes that run after this
/// one in the pipeline can retrieve the cached result with:
///
///   void getAnalysisUsage(AnalysisUsage &AU) const override {
///     AU.addRequired<RISCVBranchSupportAnalysisWrapper>();
///     AU.setPreservesAll();
///   }
///   ...
///   const RISCVBranchSupportInfo &Info =
///       getAnalysis<RISCVBranchSupportAnalysisWrapper>().getInfo();
class RISCVBranchSupportAnalysisWrapper : public MachineFunctionPass {
  RISCVBranchSupportInfo Info;

public:
  static char ID;

  RISCVBranchSupportAnalysisWrapper();

  const RISCVBranchSupportInfo &getInfo() const { return Info; }

  bool runOnMachineFunction(MachineFunction &MF) override;

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  void releaseMemory() override { Info = RISCVBranchSupportInfo(); }

  void print(raw_ostream &OS, const Module *M = nullptr) const override;

  StringRef getPassName() const override {
    return "RISC-V Branch Support Analysis";
  }
};

/// New-PM analysis, exposed for tools/pipelines that build a
/// MachineFunctionAnalysisManager directly. NOTE: as of LLVM 21 the in-tree
/// `llc` RISC-V pipeline is still driven by the legacy TargetPassConfig
/// (RISCVPassConfig), so RISCVBranchSupportAnalysisWrapper above is what
/// actually runs for ordinary `llc`/`RISCVTargetMachine` codegen. This
/// class is provided so the same analysis logic is available to any
/// MIR new-PM based driver without duplicating the traversal code.
class RISCVBranchSupportAnalysis : public AnalysisInfoMixin<RISCVBranchSupportAnalysis> {
  friend AnalysisInfoMixin<RISCVBranchSupportAnalysis>;
  static AnalysisKey Key;

public:
  using Result = RISCVBranchSupportInfo;

  Result run(MachineFunction &MF, MachineFunctionAnalysisManager &MFAM);
};

/// New-PM printer, usable as `-passes=print<riscv-branch-support>` by tools
/// that wire up the MIR new-PM pipeline.
class RISCVBranchSupportAnalysisPrinterPass
    : public PassInfoMixin<RISCVBranchSupportAnalysisPrinterPass> {
  raw_ostream &OS;

public:
  explicit RISCVBranchSupportAnalysisPrinterPass(raw_ostream &OS) : OS(OS) {}

  PreservedAnalyses run(MachineFunction &MF,
                        MachineFunctionAnalysisManager &MFAM);

  static bool isRequired() { return true; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_RISCV_RISCVBRANCHSUPPORTANALYSIS_H