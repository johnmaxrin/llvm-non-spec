# The LLVM Compiler Infrastructure

Non-speculative RISCV modification to the [LLVM Project](https://github.com/llvm/llvm-project).

## My CMake Setup
```bash
cmake -S llvm -B build -G Ninja -DLLVM_ENABLE_PROJECTS=clang;lld -DLLVM_ENABLE_RUNTIMES=compiler-rt;libc;libcxx;libcxxabi -DCLANG_DEFAULT_RTLIB=compiler-rt -DCLANG_DEFAULT_UNWINDLIB=libunwind -DCLANG_DEFAULT_C_STDLIB=libc -DCLANG_DEFAULT_CXX_STDLIB=libc++ -DCLANG_DEFAULT_LINKER=lld -DLLVM_ENABLE_LLD=ON -DLLVM_TARGETS_TO_BUILD:STRING=RISCV -DLLVM_DEFAULT_TARGET_TRIPLE:STRING=riscv64-unknown-linux-gnu -DLLVM_ENABLE_RUNTIMES=compiler-rt;libunwind;libc;libcxx;libcxxabi -DRUNTIMES_riscv64-unknown-linux-gnu_CMAKE_SYSTEM_NAME=Linux -DRUNTIMES_riscv64-unknown-linux-gnu_CMAKE_C_COMPILER_TARGET=riscv64-unknown-linux-gnu -DRUNTIMES_riscv64-unknown-linux-gnu_LLVM_ENABLE_RUNTIMES=compiler-rt;libunwind;libc;libcxx;libcxxabi
```

## How to Build
```bash
ninja -C build llc llvm-mc llvm-objdump
```

List of Implicit LLVM Assumptions that are not documented anywhere:
- Branch instructions will always hold their branch target
- All terminators of a basic block will be at the end of a basic block
- All Phi instructions must be at the beginning of a basic block

# NOTES TO SELF
- Try reverting back to old MachineBasicBlock.cpp, I don't think my changes matter anymore..

