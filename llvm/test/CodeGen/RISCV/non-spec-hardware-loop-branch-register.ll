; RUN: llc -mtriple=riscv64 -O2 -o - %s | FileCheck %s

define void @loop(ptr %values) {
entry:
  br label %loop

loop:
  %i = phi i64 [ 0, %entry ], [ %next, %loop ]
  %ptr = getelementptr i32, ptr %values, i64 %i
  store volatile i32 1, ptr %ptr
  %next = add nuw i64 %i, 1
  %done = icmp eq i64 %next, 20
  br i1 %done, label %exit, label %loop

exit:
  ret void
}

; CHECK-LABEL: loop:
; CHECK:       bmovs [[BREG:b[0-9]+]], [[LOOP:.Lns_loop_[0-9]+]]
; CHECK:       bmovt [[BREG]], {{.LBB[0-9_]+}}
; CHECK:       bmovc_loop [[BREG]],
; CHECK:       [[LOOP]]:
; CHECK-NEXT:  pb [[BREG]]
