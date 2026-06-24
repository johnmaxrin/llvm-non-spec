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
  class RISCVNS {
  public:
    static bool UseVirtualRegisters; // TODO: This is a hack, find better solution
    /*
    if (MF.getProperties().hasProperty(MachineFunctionProperties::Property::NoVRegs)) {
        // TODO: test this ^
    }
    */
  public:
    static bool isBMOVC(unsigned opcode);
    static bool isPB(unsigned opcode);
  public:
    /// Insert Unconditional branch into \p MBB. Replaces the instruction \p MI,
    /// and places a jump instruction to \p TargetBB. \p SymbolName is the name
    /// to give to the label created that points to the PB instruction.
    /// Unconditional branches are of the form:
    /// ```
    ///     bmovs bN, SymbolName
    ///     bmovt bN, TargetBB
    /// SymbolName:
    ///     pb    bN
    /// ```
    static void insertUnconditionalBranch(MachineBasicBlock& MBB,
                                          MachineInstr* MI,
                                          MachineBasicBlock* TargetBB,
                                          const char *SymbolName);

    /// Insert Unconditional branch into \p MBB with debug location \p DL.
    /// The new instruction unconditionally branches to \p TargetBB.
    /// \p SymbolName is the name to give the label created that points
    /// to the PB instruction. \p BytesAdded is an optional parameter that
    /// accumulated the total instruction bytes added.
    static void insertUnconditionalBranch(MachineBasicBlock& MBB,
                                          DebugLoc DL,
                                          MachineBasicBlock* TargetBB,
                                          const char *SymbolName,
                                          int* BytesAdded);

    /// Insert Unconditional branch into \p MBB. Replaces the instruction \p MI,
    /// and places a jump instruction to \p TargetBB. Label is automatically
    /// determined from condition code. The two registers compared are \p rs1
    /// and \p rs2. Conditional branches are of the form:
    /// ```
    ///     bmovs    bN, SymbolName
    ///     bmovt    bN, TargetBB
    ///     bmovc_CC bN, rs1, rs2
    /// SymbolName:
    ///     pb       bN
    /// ```
    static void insertConditionalBranch(MachineBasicBlock& MBB,
                                        MachineInstr* MI,
                                        Register rs1,
                                        Register rs2,
                                        MachineBasicBlock* TargetBB);

    /// Insert Unconditional branch into \p MBB with debug location \p DL
    /// and condition \p CC. The new instruction unconditionally branches
    /// to \p TargetBB. Label is automatically determined from condition code.
    /// The two registers compared are \p rs1 and \p rs2. \p BytesAdded is an
    /// optional parameter that accumulated the total instruction bytes added.
    static void insertConditionalBranch(MachineBasicBlock& MBB,
                                        DebugLoc DL,
                                        RISCVCC::CondCode CC,
                                        Register rs1,
                                        Register rs2,
                                        MachineBasicBlock* TargetBB,
                                        int* BytesAdded);

    struct BMOVSupport {
      MachineInstr *source;
      MachineInstr *target;
      MachineInstr *condition; // can be nullptr
      MachineBasicBlock *targetbb;
    };
    
    /// Returns all supporting BMOVs for a PB instruction \p PB.
    static BMOVSupport getBMOVSupport(MachineInstr* PB);

    /// Removes PB instruction \p PB and all supporting BMOVs. Optionally, also
    /// accumulate the number of bytes removed into \p BytesRemoved. Returns
    /// the number of instructions that were removed.
    static unsigned removeBranchComplete(MachineInstr* PB, int *BytesRemoved = nullptr);

    // TODO(mitch): refactor?
    static MCSymbol* getBranchSource(MachineInstr* PB);
    static void fixBranchSource(MachineInstr *BMOVS);
    static void fixBranchTarget(MachineInstr *BMOVT);
  };
} // end namespace llvm

#endif
