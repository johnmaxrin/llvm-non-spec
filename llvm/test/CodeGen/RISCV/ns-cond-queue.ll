; RUN: llc -mtriple=riscv64 -O2 -stop-after=finalize-isel < %s \
; RUN:   | FileCheck %s --check-prefix=ISEL
; RUN: llc -mtriple=riscv64 -O2 -riscv-ns-swp-distance=2 < %s \
; RUN:   | FileCheck %s --check-prefix=ASM

; Selection and expansion of the llvm.riscv.ns.cond.* intrinsics: one branch
; register per queue, with every push testing the branch's sense.

declare void @llvm.riscv.ns.cond.init(i32 immarg)
declare void @llvm.riscv.ns.cond.push.i64(i32 immarg, i32 immarg, i64, i64)
declare i1 @llvm.riscv.ns.cond.pop(i32 immarg)

; Predicates without a BMOVC of their own swap their operands. The branch is
; taken when the popped condition is false (its true successor is the layout
; successor), so each push is expanded to test the opposite condition.
define void @queue(i64 %a, i64 %b, ptr %p) {
; ISEL-LABEL: name: queue
; ISEL:         [[B:%[0-9]+]]:gpr = COPY $x11
; ISEL-NEXT:    [[A:%[0-9]+]]:gpr = COPY $x10
; ISEL-NEXT:    PseudoNSQInit 7
; ISEL-NEXT:    [[ZERO:%[0-9]+]]:gpr = COPY $x0
; ISEL-NEXT:    PseudoNSQPush 7, [[ZERO]], [[A]], 2
; ISEL-NEXT:    PseudoNSQPush 7, [[B]], [[A]], 5
; ISEL-NEXT:    PseudoNSQPush 7, [[A]], [[B]], 4
; ISEL-NEXT:    PseudoNSQBR 7, 1, %bb.2
;
; ASM-LABEL: queue:
; ASM:         bmovs [[Q:b[0-9]+]], [[LABEL:.Lns_q_[0-9]+]]
; ASM-NEXT:    bmovt [[Q]], [[TARGET:.LBB[0-9_]+]]
; ASM-NEXT:    bmovc_bge [[Q]], zero, a0
; ASM-NEXT:    bmovc_bltu [[Q]], a1, a0
; ASM-NEXT:    bmovc_bgeu [[Q]], a0, a1
; ASM-NEXT:  [[LABEL]]:
; ASM-NEXT:    pb [[Q]]
; ASM:       [[TARGET]]:
entry:
  call void @llvm.riscv.ns.cond.init(i32 7)
  ; a > 0
  call void @llvm.riscv.ns.cond.push.i64(i32 7, i32 38, i64 %a, i64 0)
  ; a <= b (unsigned)
  call void @llvm.riscv.ns.cond.push.i64(i32 7, i32 37, i64 %a, i64 %b)
  ; a < b (unsigned)
  call void @llvm.riscv.ns.cond.push.i64(i32 7, i32 36, i64 %a, i64 %b)
  %c = call i1 @llvm.riscv.ns.cond.pop(i32 7)
  br i1 %c, label %then, label %exit

then:
  store i64 1, ptr %p
  br label %exit

exit:
  ret void
}

; for (i = 0; i < n; ++i) if (a[i] > 0) a[i] += 1;
; computed two iterations ahead.
define void @inc_pos(ptr %a, i64 %n) {
; ASM-LABEL: inc_pos:
; Reset the queue, then queue the conditions of iterations 0 and 1.
; ASM:         bmovs [[Q:b[0-9]+]], [[LABEL:.Lns_q_[0-9]+]]
; ASM-NEXT:    bmovt [[Q]], [[SKIP:.LBB[0-9_]+]]
; ASM-NEXT:    lw [[V0:a[0-9]+]], 0(a0)
; ASM-NEXT:    bmovc_bge [[Q]], zero, [[V0]]
; ASM-NEXT:    lw [[V1:a[0-9]+]], 4(a0)
; ASM:         bmovc_bge [[Q]], zero, [[V1]]
; The kernel queues the condition of the element two ahead and consumes the
; one queued two iterations ago.
; ASM:         lw [[VA:a[0-9]+]], 8([[P:a[0-9]+]])
; ASM-NEXT:    bmovc_bge [[Q]], zero, [[VA]]
; ASM-NEXT:  [[LABEL]]:
; ASM-NEXT:    pb [[Q]]
; ASM:         lw {{a[0-9]+}}, 0([[P]])
; ASM:         sw {{a[0-9]+}}, 0([[P]])
; No other instruction touches the queue's branch register.
; ASM-NOT:     [[Q]]
; ASM:       .Lfunc_end1:
entry:
  %guard = icmp sgt i64 %n, 0
  br i1 %guard, label %ph, label %exit

ph:
  br label %loop

loop:
  %i = phi i64 [ 0, %ph ], [ %i.next, %latch ]
  %p = getelementptr inbounds i32, ptr %a, i64 %i
  %v = load i32, ptr %p
  %c = icmp sgt i32 %v, 0
  br i1 %c, label %then, label %latch

then:
  %v1 = add i32 %v, 1
  store i32 %v1, ptr %p
  br label %latch

latch:
  %i.next = add nuw i64 %i, 1
  %done = icmp eq i64 %i.next, %n
  br i1 %done, label %exit, label %loop

exit:
  ret void
}

; An ordinary conditional branch is set up by one BMOVC, which reads and
; writes its register: it is no queue, so its setup is still hoisted.
define void @two_ifs(ptr %p, i64 %a, i64 %b) {
; ASM-LABEL: two_ifs:
; ASM:         bmovs [[B0:b[0-9]+]], [[L0:.Lns_bge_[0-9]+]]
; ASM-NEXT:    bmovt [[B0]], {{.LBB[0-9_]+}}
; ASM-NEXT:    bmovs [[B1:b[0-9]+]], [[L1:.Lns_blt_[0-9]+]]
; ASM-NEXT:    bmovt [[B1]], {{.LBB[0-9_]+}}
; ASM:         bmovc_bge [[B0]], zero, a1
; ASM-NEXT:  [[L0]]:
; ASM-NEXT:    pb [[B0]]
; ASM:         bmovc_blt [[B1]], a2, {{a[0-9]+}}
; ASM-NEXT:  [[L1]]:
; ASM-NEXT:    pb [[B1]]
entry:
  %c1 = icmp sgt i64 %a, 0
  br i1 %c1, label %t1, label %m

t1:
  store i64 1, ptr %p
  br label %m

m:
  %c2 = icmp sgt i64 %b, 3
  br i1 %c2, label %t2, label %e

t2:
  %q = getelementptr i64, ptr %p, i64 1
  store i64 2, ptr %q
  br label %e

e:
  ret void
}
