# RUN: llvm-mc -triple=riscv64 -show-encoding %s | FileCheck %s
bpop b3, 8(sp)      # CHECK: encoding: [0xdb,0x21,0x81,0x00]
bpush b3, 8(sp)     # CHECK: encoding: [0x5b,0x34,0x31,0x00]
bpush b31, -48(sp)  # CHECK: encoding: [0x5b,0x38,0xf1,0xfd]
