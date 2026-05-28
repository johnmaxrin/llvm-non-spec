//=- RISCVMachineFunctionInfo.cpp - RISC-V machine function info --*- C++ -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares RISCV-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#include "RISCVMachineFunctionInfo.h"
#include "RISCVNonSpec.h"

#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCContext.h"

using namespace llvm;

yaml::RISCVMachineFunctionInfo::RISCVMachineFunctionInfo(
    const llvm::RISCVMachineFunctionInfo &MFI)
    : VarArgsFrameIndex(MFI.getVarArgsFrameIndex()),
      VarArgsSaveSize(MFI.getVarArgsSaveSize()) {}

MachineFunctionInfo *RISCVMachineFunctionInfo::clone(
    BumpPtrAllocator &Allocator, MachineFunction &DestMF,
    const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
    const {
  return DestMF.cloneInfo<RISCVMachineFunctionInfo>(*this);
}

RISCVMachineFunctionInfo::RISCVMachineFunctionInfo(const Function &F,
                                                   const RISCVSubtarget *STI) {

  // The default stack probe size is 4096 if the function has no
  // stack-probe-size attribute. This is a safe default because it is the
  // smallest possible guard page size.
  uint64_t ProbeSize = 4096;
  if (F.hasFnAttribute("stack-probe-size"))
    ProbeSize = F.getFnAttributeAsParsedInteger("stack-probe-size");
  else if (const auto *PS = mdconst::extract_or_null<ConstantInt>(
               F.getParent()->getModuleFlag("stack-probe-size")))
    ProbeSize = PS->getZExtValue();
  assert(int64_t(ProbeSize) > 0 && "Invalid stack probe size");

  // Round down to the stack alignment.
  uint64_t StackAlign =
      STI->getFrameLowering()->getTransientStackAlign().value();
  ProbeSize = std::max(StackAlign, alignDown(ProbeSize, StackAlign));
  StringRef ProbeKind;
  if (F.hasFnAttribute("probe-stack"))
    ProbeKind = F.getFnAttribute("probe-stack").getValueAsString();
  else if (const auto *PS = dyn_cast_or_null<MDString>(
               F.getParent()->getModuleFlag("probe-stack")))
    ProbeKind = PS->getString();
  if (ProbeKind.size()) {
    StackProbeSize = ProbeSize;
  }
}

RISCVMachineFunctionInfo::InterruptStackKind
RISCVMachineFunctionInfo::getInterruptStackKind(
    const MachineFunction &MF) const {
  if (!MF.getFunction().hasFnAttribute("interrupt"))
    return InterruptStackKind::None;

  assert(VarArgsSaveSize == 0 &&
         "Interrupt functions should not having incoming varargs");

  StringRef InterruptVal =
      MF.getFunction().getFnAttribute("interrupt").getValueAsString();

  return StringSwitch<RISCVMachineFunctionInfo::InterruptStackKind>(
             InterruptVal)
      .Case("qci-nest", InterruptStackKind::QCINest)
      .Case("qci-nonest", InterruptStackKind::QCINoNest)
      .Case("SiFive-CLIC-preemptible",
            InterruptStackKind::SiFiveCLICPreemptible)
      .Case("SiFive-CLIC-stack-swap", InterruptStackKind::SiFiveCLICStackSwap)
      .Case("SiFive-CLIC-preemptible-stack-swap",
            InterruptStackKind::SiFiveCLICPreemptibleStackSwap)
      .Default(InterruptStackKind::None);
}

void yaml::RISCVMachineFunctionInfo::mappingImpl(yaml::IO &YamlIO) {
  MappingTraits<RISCVMachineFunctionInfo>::mapping(YamlIO, *this);
}

RISCVMachineFunctionInfo::PushPopKind
RISCVMachineFunctionInfo::getPushPopKind(const MachineFunction &MF) const {
  // We cannot use fixed locations for the callee saved spill slots if the
  // function uses a varargs save area.
  // TODO: Use a separate placement for vararg registers to enable Zcmp.
  if (VarArgsSaveSize != 0)
    return PushPopKind::None;

  // SiFive interrupts are not compatible with push/pop.
  if (useSiFiveInterrupt(MF))
    return PushPopKind::None;

  // Zcmp is not compatible with the frame pointer convention.
  if (MF.getSubtarget<RISCVSubtarget>().hasStdExtZcmp() &&
      !MF.getTarget().Options.DisableFramePointerElim(MF))
    return PushPopKind::StdExtZcmp;

  // Xqccmp is Zcmp but has a push order compatible with the frame-pointer
  // convention.
  if (MF.getSubtarget<RISCVSubtarget>().hasVendorXqccmp())
    return PushPopKind::VendorXqccmp;

  return PushPopKind::None;
}

bool RISCVMachineFunctionInfo::hasImplicitFPUpdates(
    const MachineFunction &MF) const {
  switch (getInterruptStackKind(MF)) {
  case InterruptStackKind::QCINest:
  case InterruptStackKind::QCINoNest:
    // QC.C.MIENTER and QC.C.MIENTER.NEST both update FP on function entry.
    return true;
  default:
    break;
  }

  switch (getPushPopKind(MF)) {
  case PushPopKind::VendorXqccmp:
    // When using Xqccmp, we will use `QC.CM.PUSHFP` when Frame Pointers are
    // enabled, which will update FP.
    return true;
  default:
    break;
  }

  return false;
}

void RISCVMachineFunctionInfo::initializeBaseYamlFields(
    const yaml::RISCVMachineFunctionInfo &YamlMFI) {
  VarArgsFrameIndex = YamlMFI.VarArgsFrameIndex;
  VarArgsSaveSize = YamlMFI.VarArgsSaveSize;
}

void RISCVMachineFunctionInfo::addSExt32Register(Register Reg) {
  SExt32Registers.push_back(Reg);
}

bool RISCVMachineFunctionInfo::isSExt32Register(Register Reg) const {
  return is_contained(SExt32Registers, Reg);
}

#define DEBUG_TYPE "ir"

void RISCVMachineFunctionInfo::setBranch(MachineInstr* PB, MachineInstr* Source, MachineInstr* Target, MachineInstr* Condition) {
  //assert(Source->getOperand(1).isMCSymbol());
  assert(Target->getOperand(1).isMBB());
  assert((Condition == nullptr) || (Condition->getOperand(0).isReg() && Condition->getOperand(1).isReg()));
  // BMOVSupportMap[BranchIndex] = BMOVSupport { Source, Target, Condition };
  PB->getOperand(1).setImm(1);
}
MCSymbol* RISCVMachineFunctionInfo::getBranchSource(MachineInstr* PB) const {
  MachineBasicBlock *MBB = PB->getParent();
  Register Reg = PB->getOperand(0).getReg();
  for (auto It = PB->getReverseIterator(); It != MBB->rend(); ++It) {
    MachineInstr& MI = *It;
    if (MI.getOpcode() != RISCV::BMOVS_J)
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;
    return getBranchSource(PB, &MI);
  }
  llvm_unreachable("[non-spec] :(");  //[TODO] Add some better error message. 
}
void RISCVMachineFunctionInfo::fixBranchSource(MachineInstr *BMOVS) const {
  MachineBasicBlock *MBB = BMOVS->getParent();
  Register Reg = BMOVS->getOperand(0).getReg();
  for (auto It = BMOVS->getIterator(); It != MBB->end(); ++It) {
    MachineInstr& MI = *It;
    if (!RISCVNonSpec::isPB(MI.getOpcode()))
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;
    getBranchSource(&MI, BMOVS);
    return;
  }
  llvm_unreachable("[non-spec] :("); //[TODO] Add some better error message. 
}
void RISCVMachineFunctionInfo::fixBranchTarget(MachineInstr *BMOVT) const {
  MachineBasicBlock *MBB = BMOVT->getParent();
  Register Reg = BMOVT->getOperand(0).getReg();
  for (auto It = BMOVT->getIterator(); It != MBB->end(); ++It) {
    MachineInstr& MI = *It;
    if (!RISCVNonSpec::isPB(MI.getOpcode()))
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;

    assert(RISCVNonSpec::isPB(MI.getOpcode()));
    MachineBasicBlock *TargetMBB = MI.getOperand(2).getMBB();
    BMOVT->getOperand(1).setMBB(TargetMBB);
    return;
  }
  llvm_unreachable("[non-spec] :("); //[TODO] Add some better error message. 
}
MCSymbol* RISCVMachineFunctionInfo::getBranchSource(MachineInstr *PB, MachineInstr *BMOVS) const {
  MachineOperand& Operand = BMOVS->getOperand(1);
  if (Operand.isMCSymbol()) {
    return BMOVS->getOperand(1).getMCSymbol();
  }
  const char* SymbolName = Operand.getSymbolName();
  BMOVS->removeOperand(1);
  MCContext &Context = BMOVS->getParent()->getParent()->getContext();
  MCSymbol *Sym = Context.createTempSymbol(SymbolName);
  BMOVS->addOperand(MachineOperand::CreateMCSymbol(Sym));
  return Sym;
}
MachineBasicBlock* RISCVMachineFunctionInfo::getBranchTarget(const MachineInstr* PB) {
  return PB->getOperand(2).getMBB();
}
unsigned RISCVMachineFunctionInfo::getBranchOpcode(const MachineInstr *PB) const {
  const MachineBasicBlock *MBB = PB->getParent();
  Register Reg = PB->getOperand(0).getReg();
  for (auto It = PB->getReverseIterator(); It != MBB->rend(); ++It) {
    const MachineInstr& MI = *It;
    if (!RISCVNonSpec::isBMOVC(MI.getOpcode()))
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;
    return MI.getOpcode();
  }
  llvm_unreachable("[non-spec] :("); //[TODO] Add some better error message. 
}
RISCVCC::CondCode RISCVMachineFunctionInfo::getBranchCond(const MachineInstr* PB) const {
  return RISCVInstrInfo::getCondFromBranchOpc(getBranchOpcode(PB));
}
const MachineOperand& RISCVMachineFunctionInfo::getBranchReg(const MachineInstr* PB, int Index) const {
  const MachineBasicBlock *MBB = PB->getParent();
  Register Reg = PB->getOperand(0).getReg();
  for (auto It = PB->getReverseIterator(); It != MBB->rend(); ++It) {
    const MachineInstr& MI = *It;
    if (!RISCVNonSpec::isBMOVC(MI.getOpcode()))
      continue;
    if (MI.getOperand(0).getReg() != Reg)
      continue;
    return MI.getOperand(1 + Index);
  }
  llvm_unreachable("[non-spec] :("); //[TODO] Add some better error message. 
}
unsigned RISCVMachineFunctionInfo::removeBranchComplete(MachineInstr* PB, int *BytesRemoved) {
  BMOVSupport Support = getBMOVSupport(PB);
  PB->getOperand(1).setImm(0);
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

RISCVMachineFunctionInfo::BMOVSupport RISCVMachineFunctionInfo::getBMOVSupport(MachineInstr *PB) const {
  BMOVSupport Support = {};
  bool WantCondition = PB->getOpcode() == RISCV::PseudoPBC;

  MachineBasicBlock *MBB = PB->getParent();
  Register Reg = PB->getOperand(0).getReg();
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

#undef DEBUG_TYPE
