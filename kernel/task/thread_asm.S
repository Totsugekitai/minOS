.intel_syntax       noprefix

.global   init_stack
.align      16
init_stack:  # uint64_t init_stack(uint64_t *stack_bottom, uint64_t rip);
    cli
    movq      rcx,rsp   # rspを保存
    movq      rdx,rbp   # rbpを保存
	movq      rsp,rdi   # rspを移動
# iretqで必要な分
	pushq     0x10      # ss
	pushq     rdi       # rsp
	pushq     0x202     # rflags
	pushq     8         # cs
	pushq     rsi       # rip
# 割り込みハンドラが積む分
    sub       rsp,0x8
	pushq     rsp       # rbp
    add       rsp,0x8
    mov       rbp,rsp
	pushq     0         # rax
	pushq     0         # r11
	pushq     0         # r10
	pushq     0         # r9
	pushq     0         # r8
	pushq     0         # rdi
	pushq     0         # rsi
	pushq     0         # rdx
	pushq     0         # rcx
    sub       rsp,0x118
    movq      rsi,0
    movq      xmm0,rsi
    movaps    XMMWORD PTR [rbp-0x60],xmm0       # xmm15
    movaps    XMMWORD PTR [rbp-0x70],xmm0       # xmm14
    movaps    XMMWORD PTR [rbp-0x80],xmm0       # xmm13
    movaps    XMMWORD PTR [rbp-0x90],xmm0       # xmm12
    movaps    XMMWORD PTR [rbp-0xa0],xmm0       # xmm11
    movaps    XMMWORD PTR [rbp-0xb0],xmm0       # xmm10
    movaps    XMMWORD PTR [rbp-0xc0],xmm0       # xmm9
    movaps    XMMWORD PTR [rbp-0xd0],xmm0       # xmm8
    movaps    XMMWORD PTR [rbp-0xe0],xmm0       # xmm7
    movaps    XMMWORD PTR [rbp-0xf0],xmm0       # xmm6
    movaps    XMMWORD PTR [rbp-0x100],xmm0      # xmm5
    movaps    XMMWORD PTR [rbp-0x110],xmm0      # xmm4
    movaps    XMMWORD PTR [rbp-0x120],xmm0      # xmm3
    movaps    XMMWORD PTR [rbp-0x130],xmm0      # xmm2
    movaps    XMMWORD PTR [rbp-0x140],xmm0      # xmm1
    movaps    XMMWORD PTR [rbp-0x150],xmm0      # xmm0
	movq      rax,rsp   # rspを戻り値に設定
	movq      rsp,rcx   # rspを復旧
    movq      rbp,rdx   # rbpを復旧
    sti
	ret

.global     dispatch
.align      16
dispatch:  # void dispatch(uint64_t next_rsp, uint64_t *current_rsp, uint64_t next_rip);
    movq      [rsi],rsp   # rspを保存
    movq      rsp,rdi     # rspを更新
    movaps    xmm0,XMMWORD PTR  [rsp+0x10]
    movaps    xmm1,XMMWORD PTR  [rsp+0x20]
    movaps    xmm2,XMMWORD PTR  [rsp+0x30]
    movaps    xmm3,XMMWORD PTR  [rsp+0x40]
    movaps    xmm4,XMMWORD PTR  [rsp+0x50]
    movaps    xmm5,XMMWORD PTR  [rsp+0x60]
    movaps    xmm6,XMMWORD PTR  [rsp+0x70]
    movaps    xmm7,XMMWORD PTR  [rsp+0x80]
    movaps    xmm8,XMMWORD PTR  [rsp+0x90]
    movaps    xmm9,XMMWORD PTR  [rsp+0xa0]
    movaps    xmm10,XMMWORD PTR [rsp+0xb0]
    movaps    xmm11,XMMWORD PTR [rsp+0xc0]
    movaps    xmm12,XMMWORD PTR [rsp+0xd0]
    movaps    xmm13,XMMWORD PTR [rsp+0xe0]
    movaps    xmm14,XMMWORD PTR [rsp+0xf0]
    movaps    xmm15,XMMWORD PTR [rsp+0x100]
    add       rsp,0x118
    popq      rcx
    popq      rax
    popq      rsi
    popq      rdi
    popq      r8
    popq      r9
    popq      r10
    popq      r11
    popq      rax
    popq      rbp
# interrupt stack frame
    mov       rdi,rsp
	pushq     0x10      # ss
	pushq     rdi       # rsp
	pushq     0x202     # rflags
	pushq     8         # cs
	pushq     rdx       # rip
    iretq
