//===-- RISCVNonSpec.h - RISC-V Non Spec Interface --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces for the Non-Spec RISC-V research project
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_RISCVNONSPEC_H
#define LLVM_LIB_TARGET_RISCV_RISCVNONSPEC_H

#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"

namespace llvm {
  class RISCVNonSpec {
  public:
    static bool UseVirtualRegisters;
  public:
    static void insertUnconditionalBranch(MachineBasicBlock& MBB,
                                          MachineInstr* MI,
                                          MachineBasicBlock* TargetBB,
                                          const char *SymbolName);
    static void insertUnconditionalBranch(MachineBasicBlock& MBB,
                                          DebugLoc DL,
                                          MachineBasicBlock* TargetBB,
                                          const char *SymbolName,
                                          int* BytesAdded);
    static void insertConditionalBranch(MachineBasicBlock& MBB,
                                        MachineInstr* MI,
                                        Register rs1,
                                        Register rs2,
                                        MachineBasicBlock* TargetBB);
    static void insertConditionalBranch(MachineBasicBlock& MBB,
                                        DebugLoc DL,
                                        RISCVCC::CondCode CC,
                                        Register rs1,
                                        Register rs2,
                                        MachineBasicBlock* TargetBB,
                                        int* BytesAdded);
  };
} // end namespace llvm

#endif
