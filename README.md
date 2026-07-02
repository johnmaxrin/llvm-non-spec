# The LLVM Compiler Infrastructure

Non-speculative RISCV modification to the [LLVM Project](https://github.com/llvm/llvm-project).

## My CMake Setup
```bash
-G Ninja
-DCMAKE_C_COMPILER=clang
-DCMAKE_CXX_COMPILER=clang++
-DCMAKE_BUILD_TYPE=Debug
"-DLLVM_ENABLE_PROJECTS=clang;lld"
-DLLVM_TARGETS_TO_BUILD=RISCV
-DLLVM_DEFAULT_TARGET_TRIPLE=riscv64-unknown-linux-gnu
-DLLVM_ENABLE_LLD=ON
-DLLVM_LIBC_FULL_BUILD=ON
-DCLANG_DEFAULT_RTLIB=compiler-rt
-DCLANG_DEFAULT_UNWINDLIB=libunwind
-DCLANG_DEFAULT_C_STDLIB=libc
-DCLANG_DEFAULT_CXX_STDLIB=libc++
-DCLANG_DEFAULT_LINKER=lld
-DLLVM_RUNTIME_TARGETS=riscv64-linux-gnu
"-DLLVM_ENABLE_RUNTIMES=compiler-rt;libunwind;libc;libcxx;libcxxabi"
-DRUNTIMES_riscv64-linux-gnu_LLVM_LIBC_FULL_BUILD=ON
-DRUNTIMES_riscv64-linux-gnu_LIBC_TARGET_TRIPLE=riscv64-unknown-linux-gnu
-DRUNTIMES_riscv64-linux-gnu_LIBC_KERNEL_HEADERS=/home/mitchell/riscv-kernel-headers/include
"-DRUNTIMES_riscv64-linux-gnu_LLVM_ENABLE_RUNTIMES=compiler-rt;libunwind;libc;libcxx;libcxxabi"
```

## How to Build
```bash
ninja -C build llc llvm-mc llvm-objdump
```

List of Implicit LLVM Assumptions that are not documented anywhere:
- Branch instructions will always hold their branch target
- All terminators of a basic block will be at the end of a basic block
- All Phi instructions must be at the beginning of a basic block
- LLVM will just copy MachineInstr's as it pleases without letting you know

# NOTES TO SELF
- Try reverting back to old MachineBasicBlock.cpp, I don't think my changes matter anymore..

