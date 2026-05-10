#include "NonSpecTargetMachine.h"

#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/MC/TargetRegistry.h" 

#include "TargetInfo/NonSpecTargetInfo.h"

using namespace llvm;


static const char *NonSpecDataLayoutStr = 
    "some-random-string-here!";

NonSpecTargetMachine::NonSpecTargetMachine(const Target &T,
            const Triple &TT,
            StringRef CPU,
            StringRef FS,
            const TargetOptions &Options,
            std::optional<Reloc::Model> RM,
            std::optional<CodeModel::Model> CM,
            CodeGenOptLevel OL,
            bool JIT) : CodeGenTargetMachineImpl(T, NonSpecDataLayoutStr, TT, CPU, FS, Options, RM? *RM : Reloc::Static, CM? *CM : CodeModel::Small, OL)
            {}

NonSpecTargetMachine::~NonSpecTargetMachine() = default;



extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void LLVMInitializeNonSpecTarget() {
    RegisterTargetMachine<NonSpecTargetMachine> X(getTheNonSpecTarget32());
    RegisterTargetMachine<NonSpecTargetMachine> Y(getTheNonSpecTarget64());
}