# The LLVM Compiler Infrastructure

Non-speculative RISCV modification to the [LLVM Project](https://github.com/llvm/llvm-project).

## Setup
```bash
cmake -S llvm -B build -G Ninja -DLLVM_TARGETS_TO_BUILD=RISCV -DLLVM_DEFAULT_TARGET_TRIPLE=riscv64-linux-elf -DCMAKE_BUILD_TYPE=Release
```

## Build
```bash
ninja -C build llc llvm-mc llvm-objdump
```

