#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"

#include "MCTargetDesc/RISCVMCTargetDesc.h"
#include "RISCV.h"
#include "RISCVInstrInfo.h"
#include "RISCVSubtarget.h"
#include "RISCVTargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "riscv-ns-branch-opt"
#define PASS_NAME "riscv-ns-branch-opt"

namespace {
class RISCVNSBranchOpt : public MachineFunctionPass {

public:
  static char ID;
  RISCVNSBranchOpt() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    bool Changed = false;
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    LLVM_DEBUG(dbgs() << "RISCV Non Speculative Branch Optimization\n");

    for (MachineBasicBlock &MBB : MF) {
      Changed |= optimizeBlock(MBB, TII);
    }

    return Changed;
  }

  StringRef getPassName() const override {
    return "RISCV NS Branch Optimization";
  }

private:
  bool optimizeBlock(MachineBasicBlock &MBB, const TargetInstrInfo *TII) {
    bool Changed = false;
    int Cbmovs = 0;
    int Cbmovt = 0;
    int Cbmovc = 0;
    int Cpb = 0;

    for (auto I = MBB.begin(), E = MBB.end(); I != E;) {
      MachineInstr &MI = *I++;

    //   LLVM_DEBUG(
    //       { dbgs() << "Opcode: " << TII->getName(MI.getOpcode()) << "\n"; });

      switch (MI.getOpcode()) {
      case RISCV::BMOVS_J:
        ++Cbmovs;
        MBB.splice(MBB.begin(), &MBB, MI);
        break;

      case RISCV::BMOVS_I:
        MBB.splice(MBB.begin(), &MBB, MI);
        ++Cbmovs;
        break;

      case RISCV::BMOVT_J:
        MBB.splice(MBB.begin(), &MBB, MI);
        ++Cbmovt;
        break;

      case RISCV::BMOVT_I:
        MBB.splice(MBB.begin(), &MBB, MI);
        ++Cbmovt;
        break;

      case RISCV::BMOVC_BNE:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BEQ:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BGE:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BGEU:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BLT:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BLTU:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_BITS:
        ++Cbmovc;
        break;

      case RISCV::BMOVC_LOOP:
        ++Cbmovc;
        break;

      case RISCV::PseudoPBC:
        ++Cpb;
        break;

      case RISCV::PseudoPBU:
        ++Cpb;
        break;
      }
    }

    // Ignore these for now, Just wanted to see  how things works ;>
    LLVM_DEBUG(dbgs() << "BMOVS Count: " << Cbmovs << "\n");
    LLVM_DEBUG(dbgs() << "BMOVT Count: " << Cbmovt << "\n");
    LLVM_DEBUG(dbgs() << "BMOVC Count: " << Cbmovc << "\n");
    LLVM_DEBUG(dbgs() << "PB Count: " << Cpb << "\n");

    return Changed;
  }
};
} // end anonymous namespace

char RISCVNSBranchOpt::ID = 0;

INITIALIZE_PASS(RISCVNSBranchOpt, PASS_NAME,
                "RISCV Non Speculative Branch Optimization", false, true)

FunctionPass *llvm::createRISCVNSBranchOptPass() {
  return new RISCVNSBranchOpt();
}