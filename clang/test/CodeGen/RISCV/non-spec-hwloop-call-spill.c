// RUN: %clang_cc1 -triple riscv64 -target-feature +m -O2 -S -o - %s | FileCheck %s

__attribute__((noinline)) int increment(int value) {
  return value + 1;
}

int kernel(void) {
  int total = 0;
#pragma clang loop unroll(disable)
  for (int i = 0; i < 10; i++) {
    total += increment(i);
    __asm__ volatile("" : "+r"(total));
  }
  return total;
}

// CHECK-LABEL: kernel:
// CHECK: bpush b24
// CHECK: bmovc_loop b24
// CHECK: pbal
// CHECK: pb b24
// CHECK: bpop b24
