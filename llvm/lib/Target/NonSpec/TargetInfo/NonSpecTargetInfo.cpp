#include "llvm/TextAPI/Target.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

#include "TargetInfo/NonSpecTargetInfo.h"



using namespace llvm;

Target &llvm::getTheNonSpecTarget32()
{
    static Target TheNonSpecTarget32;
    return TheNonSpecTarget32;
}

Target &llvm::getTheNonSpecTarget64()
{
    static Target TheNonSpecTarget64;
    return TheNonSpecTarget64;
}


extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeNonSpecTargetInfo() {
  RegisterTarget<Triple::riscv32_non_spec> X(getTheNonSpecTarget32(), "nonspec",
                                  "NON SPEC 32-bit", "NONSPEC");
  RegisterTarget<Triple::riscv64_non_spec> Y(getTheNonSpecTarget64(), "nonspec",
                                    "NON SPEC 64-bit", "NONSPEC");
}
