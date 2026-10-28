# This file allows users to call find_package(LLD) and pick up our targets.



set(LLVM_VERSION 21.1.8)
find_package(LLVM ${LLVM_VERSION} EXACT REQUIRED CONFIG
             HINTS "/home/robert/scratch/llvm-non-spec/./lib/cmake/llvm")

set(LLD_EXPORTED_TARGETS "lldCommon;lld;lldCOFF;lldELF;lldMachO;lldMinGW;lldWasm")
set(LLD_CMAKE_DIR "/home/robert/scratch/llvm-non-spec/lib/cmake/lld")
set(LLD_INCLUDE_DIRS "/home/robert/scratch/llvm-non-spec/lld/include;/home/robert/scratch/llvm-non-spec/tools/lld/include")

# Provide all our library targets to users.
include("/home/robert/scratch/llvm-non-spec/lib/cmake/lld/LLDTargets.cmake")
