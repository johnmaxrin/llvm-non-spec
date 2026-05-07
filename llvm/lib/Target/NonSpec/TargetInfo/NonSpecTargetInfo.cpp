#include "llvm/TextAPI/Target.h"

using namespace llvm;

Target &llvm::getTheNonSpecTarget()
{
    static Target TheNonSpecTarget;
    return TheNonSpecTarget;
}




#include "TargetInfo/NVPTXTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
using namespace llvm;

Target &llvm::getTheNVPTXTarget32() {
  static Target TheNVPTXTarget32;
  return TheNVPTXTarget32;
}
Target &llvm::getTheNVPTXTarget64() {
  static Target TheNVPTXTarget64;
  return TheNVPTXTarget64;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializeNVPTXTargetInfo() {
  RegisterTarget<Triple::nvptx> X(getTheNVPTXTarget32(), "nvptx",
                                  "NVIDIA PTX 32-bit", "NVPTX");
  RegisterTarget<Triple::nvptx64> Y(getTheNVPTXTarget64(), "nvptx64",
                                    "NVIDIA PTX 64-bit", "NVPTX");
}
