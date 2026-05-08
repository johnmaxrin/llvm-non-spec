//===-- RISCVNonSpec.h - RISC-V Non Spec Implementation ---------*- C++ -*-===//

#include "RISCVNonSpec.h"
#include "MCTargetDesc/RISCVMatInt.h"
#include "RISCVMachineFunctionInfo.h"
#include "RISCVSubtarget.h"

#include "llvm/Analysis/ValueTracking.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/SDPatternMatch.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/MC/MCContext.h"

#define DEBUG_TYPE "riscv-nonspec"

using namespace llvm;

bool RISCVNonSpec::UseVirtualRegisters = true;

// TODO: use `getCondFromBranchOpc`

static const char* labelFromCC(RISCVCC::CondCode CC) {
  switch (CC) {
  case RISCVCC::COND_EQ: return "ns_beq_";
  case RISCVCC::COND_NE: return "ns_bne_";
  case RISCVCC::COND_LT: return "ns_blt_";
  case RISCVCC::COND_GE: return "ns_bge_";
  case RISCVCC::COND_LTU: return "ns_bltu_";
  case RISCVCC::COND_GEU: return "ns_bgeu_";
  default:
    llvm_unreachable("[Non-Spec] Unexpected opcode");
  }
}

static const char* PBLabelFromPseudoInstrCC(const MachineInstr& MI) {
  switch (MI.getOpcode()) {
  case RISCV::PseudoBREQ: return "ns_beq_";
  case RISCV::PseudoBRNE: return "ns_bne_";
  case RISCV::PseudoBRLT: return "ns_blt_";
  case RISCV::PseudoBRGE: return "ns_bge_";
  case RISCV::PseudoBRLTU: return "ns_bltu_";
  case RISCV::PseudoBRGEU: return "ns_bgeu_";
  default:
    llvm_unreachable("[Non-Spec] Unexpected opcode");
  }
}

static unsigned BMOVCInstrFromCC(RISCVCC::CondCode CC) {
  switch (CC) {
  case RISCVCC::COND_EQ: return RISCV::BMOVC_BEQ;
  case RISCVCC::COND_NE: return RISCV::BMOVC_BNE;
  case RISCVCC::COND_LT: return RISCV::BMOVC_BLT;
  case RISCVCC::COND_GE: return RISCV::BMOVC_BGE;
  case RISCVCC::COND_LTU: return RISCV::BMOVC_BLTU;
  case RISCVCC::COND_GEU: return RISCV::BMOVC_BGEU;
  default:
    llvm_unreachable("[Non-Spec] Unexpected opcode");
  }
}

static unsigned BMOVCInstrFromPseudoInstr(const MachineInstr& MI) {
  switch (MI.getOpcode()) {
  case RISCV::PseudoBREQ: return RISCV::BMOVC_BEQ;
  case RISCV::PseudoBRNE: return RISCV::BMOVC_BNE;
  case RISCV::PseudoBRLT: return RISCV::BMOVC_BLT;
  case RISCV::PseudoBRGE: return RISCV::BMOVC_BGE;
  case RISCV::PseudoBRLTU: return RISCV::BMOVC_BLTU;
  case RISCV::PseudoBRGEU: return RISCV::BMOVC_BGEU;
  default:
    llvm_unreachable("[Non-Spec] Unexpected opcode");
  }
}

static bool IsNonSpec(unsigned opcode) {
  switch (opcode) {
  case RISCV::BMOVS_I:
  case RISCV::BMOVS_J:
  case RISCV::BMOVT_I:
  case RISCV::BMOVT_J:
  case RISCV::PBAL:
  case RISCV::PseudoPBC:
  case RISCV::PseudoPBU:
  case RISCV::BMOVC_BEQ:
  case RISCV::BMOVC_BNE:
  case RISCV::BMOVC_BLT:
  case RISCV::BMOVC_BGE:
  case RISCV::BMOVC_BLTU:
  case RISCV::BMOVC_BGEU:
  case RISCV::PseudoBR:
  case RISCV::PseudoBREQ:
  case RISCV::PseudoBRNE:
  case RISCV::PseudoBRLT:
  case RISCV::PseudoBRGE:
  case RISCV::PseudoBRLTU:
  case RISCV::PseudoBRGEU:
    return true;
  default:
    return false;
  }
}

static bool IsNonPseudoNonSpec(unsigned opcode) {
  switch (opcode) {
  case RISCV::BMOVS_I:
  case RISCV::BMOVS_J:
  case RISCV::BMOVT_I:
  case RISCV::BMOVT_J:
  case RISCV::PBAL:
  case RISCV::PseudoPBC:
  case RISCV::PseudoPBU:
  case RISCV::BMOVC_BEQ:
  case RISCV::BMOVC_BNE:
  case RISCV::BMOVC_BLT:
  case RISCV::BMOVC_BGE:
  case RISCV::BMOVC_BLTU:
  case RISCV::BMOVC_BGEU:
    return true;
  default:
    return false;
  }
}

bool isNonSpecOrDebugInstr(const MachineInstr& MI) {
  return IsNonSpec(MI.getOpcode()) || MI.isDebugInstr();
}

MachineBasicBlock::iterator getBMOVSupportInsertLoc(MachineBasicBlock& MBB, MachineBasicBlock::iterator EndIt, int* RegisterNo, bool insertAtEnd = false) {
  // Base case
  if (MBB.empty()) {
    return MBB.end(); // Where the hell else are we going to insert?
  }
  MachineBasicBlock::iterator InsertIt = EndIt;
  uint32_t UsedRegisters = 0;
  // Walk backwards past non-spec/debug instructions
  bool First = true;
  for (auto RI = InsertIt.getReverse(); RI != MBB.rend(); ++RI) {
    MachineInstr &MI = *RI;
    if (!isNonSpecOrDebugInstr(MI)) {
      if (First) {
        InsertIt = insertAtEnd ? MBB.end() : EndIt;
      }
      break;
    }
    if (IsNonPseudoNonSpec(MI.getOpcode())) {
      const Register& BR = MI.getOperand(0).getReg();
      if (BR.isPhysical()) {
        UsedRegisters |= (1 << (BR - RISCV::B0));
      }
    }
    InsertIt = MI.getIterator();
    First = false;
  }
  for (unsigned i = 0; i < 32; ++i) {
    if (!(UsedRegisters & (1 << i))) {
      *RegisterNo = i;
      break;
    }
  }
  return InsertIt;
}

bool RISCVNonSpec::isBMOVC(unsigned opcode) {
  switch (opcode) {
  case RISCV::BMOVC_BEQ:
  case RISCV::BMOVC_BNE:
  case RISCV::BMOVC_BLT:
  case RISCV::BMOVC_BGE:
  case RISCV::BMOVC_BLTU:
  case RISCV::BMOVC_BGEU:
    return true;
  default:
    return false;
  }
}

bool RISCVNonSpec::isPB(unsigned opcode) {
  switch (opcode) {
  case RISCV::PBAL:
  case RISCV::PseudoPBC:
  case RISCV::PseudoPBU:
    return true;
  default:
    return false;
  }
}

void RISCVNonSpec::insertUnconditionalBranch(MachineBasicBlock& MBB,
                                             MachineInstr* MI,
                                             MachineBasicBlock* TargetBB,
                                             const char *SymbolName) {
  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo *TII = MF->getSubtarget().getInstrInfo();
  const DebugLoc DL = MI->getDebugLoc();

  int RegisterNo = 0;
  MachineBasicBlock::iterator SupportIt = getBMOVSupportInsertLoc(
    MBB, MI->getIterator(), &RegisterNo, false);

  const Register BR = (UseVirtualRegisters ?
    MF->getRegInfo().createVirtualRegister(&RISCV::PBRRegClass) :
    Register(RISCV::B0 + RegisterNo));

  // bmovs b0, (location of pb)
  MachineInstr *source =
    BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVS_J))
      .addDef(BR)
      .addExternalSymbol(SymbolName);

  // bmovt b0, (target)
  MachineInstr *target =
    BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVT_J))
      .addUse(BR)
      .addMBB(TargetBB);

  // pb b0
  MachineInstr *pb =
    BuildMI(MBB, MI, DL, TII->get(RISCV::PseudoPBU))
      .addUse(BR)
      .addImm(-1) // This is where branch index will be assigned
      .addMBB(TargetBB);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  RISCVMachineFunctionInfo *MFI = MF->getInfo<RISCVMachineFunctionInfo>();
  MFI->setBranch(pb, source, target);
  MI->eraseFromParent();
}

void RISCVNonSpec::insertUnconditionalBranch(MachineBasicBlock& MBB,
                                             DebugLoc DL,
                                             MachineBasicBlock* TargetBB,
                                             const char *SymbolName,
                                             int* BytesAdded) {
  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo *TII = MF->getSubtarget().getInstrInfo();

  int RegisterNo = 0;
  MachineBasicBlock::iterator SupportIt = getBMOVSupportInsertLoc(
  MBB, MBB.getLastNonDebugInstr(), &RegisterNo, true);

  const Register BR = (UseVirtualRegisters ?
    MF->getRegInfo().createVirtualRegister(&RISCV::PBRRegClass) :
    Register(RISCV::B0 + RegisterNo));

  // bmovs b0, (location of pb)
  MachineInstr *source = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVS_J))
      .addDef(BR)
      .addExternalSymbol(SymbolName);
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*source);

  // bmovt b0, (target)
  MachineInstr *target = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVT_J))
      .addUse(BR)
      .addMBB(TargetBB);
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*target);

  // pb b0
  MachineInstr *pb = BuildMI(&MBB, DL, TII->get(RISCV::PseudoPBU))
      .addUse(BR)
      .addImm(-1) // This is where branch index will be assigned
      .addMBB(TargetBB);
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*pb);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  RISCVMachineFunctionInfo *MFI = MF->getInfo<RISCVMachineFunctionInfo>();
  MFI->setBranch(pb, source, target);
}

void RISCVNonSpec::insertConditionalBranch(MachineBasicBlock& MBB,
                                           MachineInstr* MI,
                                           Register rs1,
                                           Register rs2,
                                           MachineBasicBlock* TargetBB) {
  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo *TII = MF->getSubtarget().getInstrInfo();
  const DebugLoc DL = MI->getDebugLoc();

  int RegisterNo = 0;
  MachineBasicBlock::iterator SupportIt = getBMOVSupportInsertLoc(
  MBB, MI->getIterator(), &RegisterNo, false);

  const Register BR = (UseVirtualRegisters ?
    MF->getRegInfo().createVirtualRegister(&RISCV::PBRRegClass) :
    Register(RISCV::B0 + RegisterNo));

  // bmovs b0, (location of pb)
  MachineInstr *source = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVS_J))
      .addDef(BR)
      .addExternalSymbol(PBLabelFromPseudoInstrCC(*MI));

  // bmovt b0, (target)
  MachineInstr *target = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVT_J))
      .addUse(BR)
      .addMBB(TargetBB);

  // bmovc_bxx b0, rs1, rs2
  MachineInstr *condition = BuildMI(MBB, SupportIt, DL, TII->get(BMOVCInstrFromPseudoInstr(*MI)))
      .addUse(BR)
      .addReg(rs1)
      .addReg(rs2);

  // pb b0
  MachineInstr *pb = BuildMI(MBB, MI, DL, TII->get(RISCV::PseudoPBC))
      .addUse(BR)
      .addImm(-1) // This is where branch index will be assigned
      .addMBB(TargetBB);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  RISCVMachineFunctionInfo *MFI = MF->getInfo<RISCVMachineFunctionInfo>();
  MFI->setBranch(pb, source, target, condition);
  MI->eraseFromParent();
}

void RISCVNonSpec::insertConditionalBranch(MachineBasicBlock& MBB,
                                           DebugLoc DL,
                                           RISCVCC::CondCode CC,
                                           Register rs1,
                                           Register rs2,
                                           MachineBasicBlock* TargetBB,
                                           int* BytesAdded) {
  MachineFunction *MF = MBB.getParent();
  const TargetInstrInfo *TII = MF->getSubtarget().getInstrInfo();

  int RegisterNo = 0;
  MachineBasicBlock::iterator SupportIt = getBMOVSupportInsertLoc(
  MBB, MBB.getLastNonDebugInstr(), &RegisterNo, true);

  const Register BR = (UseVirtualRegisters ?
    MF->getRegInfo().createVirtualRegister(&RISCV::PBRRegClass) :
    Register(RISCV::B0 + RegisterNo));

  // bmovs b0, (location of pb)
  MachineInstr *source = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVS_J))
      .addDef(BR)
      .addExternalSymbol(labelFromCC(CC));
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*source);

  // bmovt b0, (target)
  MachineInstr *target = BuildMI(MBB, SupportIt, DL, TII->get(RISCV::BMOVT_J))
      .addUse(BR)
      .addMBB(TargetBB);
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*target);

  // bmovc_b[cond] b0, rs1, rs2
  MachineInstr *condition = BuildMI(MBB, SupportIt, DL, TII->get(BMOVCInstrFromCC(CC)))
      .addUse(BR)
      .addReg(rs1)
  .addReg(rs2);
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*condition);

  // pb b0
  MachineInstr *pb = BuildMI(&MBB, DL, TII->get(RISCV::PseudoPBC))
      .addUse(BR)
      .addImm(-1) // This is where branch index will be assigned
      .addMBB(TargetBB);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*pb);

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  RISCVMachineFunctionInfo *MFI = MF->getInfo<RISCVMachineFunctionInfo>();
  MFI->setBranch(pb, source, target, condition);
}
