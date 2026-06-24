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

bool RISCVNS::UseVirtualRegisters = true;

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

bool RISCVNS::isBMOVC(unsigned opcode) {
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

bool RISCVNS::isPB(unsigned opcode) {
  switch (opcode) {
  case RISCV::PBAL:
  case RISCV::PseudoPBC:
  case RISCV::PseudoPBU:
  case RISCV::PseudoPBI:
    return true;
  default:
    return false;
  }
}

static inline Register getPBBranchRegister(const MachineInstr* MI) {
  if (MI->getOpcode() == RISCV::PBAL) {
    // PBAL has branch register as second operand
    return MI->getOperand(1).getReg();
  }
  // All PseudoPBs will have branch register as first operand
  return MI->getOperand(0).getReg();
}

void RISCVNS::insertUnconditionalBranch(MachineBasicBlock& MBB,
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
      .addMBB(TargetBB);
  // NOTE(mitch): ^^ we include the target location here so
  //              that compiler passes will see this as a normal jump

  (void)sizeof(source, target);
  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  MI->eraseFromParent();
}

void RISCVNS::insertUnconditionalBranch(MachineBasicBlock& MBB,
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
      .addMBB(TargetBB);
  // NOTE(mitch): ^^ we include the target location here so
  //              that compiler passes will see this as a normal jump
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*pb);

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);
}

void RISCVNS::insertConditionalBranch(MachineBasicBlock& MBB,
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
      .addMBB(TargetBB);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump

  (void)sizeof(source, target, condition);
  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);

  MI->eraseFromParent();
}

void RISCVNS::insertConditionalBranch(MachineBasicBlock& MBB,
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
      .addMBB(TargetBB);
  // NOTE(non-spec): ^^ we include the target location here so
  //                 that compiler passes will see this as a normal jump
  if (BytesAdded)
    *BytesAdded += TII->getInstSizeInBytes(*pb);

  LLVM_DEBUG(dbgs() << __func__ << ": " << *pb);
}


RISCVNS::BMOVSupport RISCVNS::getBMOVSupport(MachineInstr *PB) {
  BMOVSupport Support = {};
  bool WantCondition = PB->getOpcode() == RISCV::PseudoPBC;

  MachineBasicBlock *MBB = PB->getParent();
  Register Reg = getPBBranchRegister(PB);
  for (auto It = PB->getReverseIterator(); It != MBB->rend(); ++It) {
    MachineInstr& MI = *It;
    switch (MI.getOpcode()) {
      case RISCV::BMOVS_J: {
        if (MI.getOperand(0).getReg() != Reg)
          continue;
        Support.source = &MI;
        break;
      }
      case RISCV::BMOVT_J: {
          if (MI.getOperand(0).getReg() != Reg)
            continue;
          Support.target = &MI;
          Support.targetbb = MI.getOperand(1).getMBB();
          break;
      }
      case RISCV::BMOVC_BEQ:
      case RISCV::BMOVC_BNE:
      case RISCV::BMOVC_BLT:
      case RISCV::BMOVC_BGE:
      case RISCV::BMOVC_BLTU:
      case RISCV::BMOVC_BGEU:
      {
          if (!WantCondition)
            continue;
          if (MI.getOperand(0).getReg() != Reg)
            continue;
          Support.condition = &MI;
          break;
      }
      default: continue;
    }
  }
  assert(Support.source != nullptr);
  assert(Support.target != nullptr);
  assert(!WantCondition || Support.condition != nullptr);
  return Support;
}

unsigned RISCVNS::removeBranchComplete(MachineInstr* PB, int *BytesRemoved) {
  BMOVSupport Support = getBMOVSupport(PB);
  unsigned NumberOfInstructionsRemoved = 0;

  const RISCVInstrInfo *TII =
    PB->getParent()->getParent()->getSubtarget<RISCVSubtarget>().getInstrInfo();

  if (BytesRemoved)
    *BytesRemoved += TII->getInstSizeInBytes(*Support.source);
  // dbgs() << ">Removing " << *Support.source;
  Support.source->eraseFromParent();
  NumberOfInstructionsRemoved += 1;
  if (BytesRemoved)
    *BytesRemoved += TII->getInstSizeInBytes(*Support.target);
  // dbgs() << ">Removing " << *Support.target;
  Support.target->eraseFromParent();
  NumberOfInstructionsRemoved += 1;
  if (Support.condition) {
    if (BytesRemoved)
      *BytesRemoved += TII->getInstSizeInBytes(*Support.condition);
    // dbgs() << ">Removing " << *Support.condition;
    Support.condition->eraseFromParent();
    NumberOfInstructionsRemoved += 1;
  }
  if (BytesRemoved)
    *BytesRemoved += TII->getInstSizeInBytes(*PB);
  // dbgs() << ">Removing " << *PB;
  PB->eraseFromParent();
  NumberOfInstructionsRemoved += 1;
  return NumberOfInstructionsRemoved;
}

MCSymbol* getBranchSourceHelper(MachineInstr *BMOVS) {
  MachineOperand& Operand = BMOVS->getOperand(1);
  if (Operand.isMCSymbol()) {
    return BMOVS->getOperand(1).getMCSymbol();
  }
  if (Operand.isImm()) {
    return nullptr;
  }
  const char* SymbolName = Operand.getSymbolName();
  BMOVS->removeOperand(1);
  MCContext &Context = BMOVS->getParent()->getParent()->getContext();
  MCSymbol *Sym = Context.createTempSymbol(SymbolName);
  BMOVS->addOperand(MachineOperand::CreateMCSymbol(Sym));
  return Sym;
}

MCSymbol* RISCVNS::getBranchSource(MachineInstr* PB) {
  MachineBasicBlock *MBB = PB->getParent();
  Register Reg = getPBBranchRegister(PB);
  for (auto It = PB->getReverseIterator(); It != MBB->rend(); ++It) {
    MachineInstr& MI = *It;
    if (MI.getOpcode() != RISCV::BMOVS_J)
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;
    return getBranchSourceHelper(&MI);
  }
  llvm_unreachable("[non-spec] :(");
}

void RISCVNS::fixBranchSource(MachineInstr *BMOVS) {
  if (BMOVS->getOperand(1).isImm()) {
    return; // probably ok right?
  }
  MachineBasicBlock *MBB = BMOVS->getParent();
  Register Reg = BMOVS->getOperand(0).getReg();
  for (auto It = BMOVS->getIterator(); It != MBB->end(); ++It) {
    MachineInstr& MI = *It;
    if (!isPB(MI.getOpcode()))
      continue;
    if (getPBBranchRegister(&MI) != Reg)
      continue;
    getBranchSourceHelper(BMOVS);
    return;
  }
  llvm_unreachable("[non-spec] :(");
}

void RISCVNS::fixBranchTarget(MachineInstr *BMOVT) {
  MachineBasicBlock *MBB = BMOVT->getParent();
  Register Reg = BMOVT->getOperand(0).getReg();
  for (auto It = BMOVT->getIterator(); It != MBB->end(); ++It) {
    MachineInstr& PB = *It;
    if (!isPB(PB.getOpcode()))
      continue;
    if (getPBBranchRegister(&PB) != Reg)
      continue;

    MachineBasicBlock *TargetMBB = PB.getOperand(1).getMBB();
    BMOVT->getOperand(1).setMBB(TargetMBB);
    return;
  }
  llvm_unreachable("[non-spec] :(");
}
