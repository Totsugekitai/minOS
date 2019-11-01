.intel_syntax       noprefix

.global     init_stack
.align      16
init_stack:  # uint64_t init_stack(uint64_t *stack_bottom, uint64_t rip);
    cli
    movq      rcx,rsp   # rspを保存
	movq      rsp,rdi   # rspを移動
# iretqで必要な分
	pushq     0x10      # ss
	pushq     rdi       # rsp
	pushq     0x202     # rflags
	pushq     8         # cs
	pushq     rsi       # rip
# 割り込みハンドラが積む分
	pushq     0         # rbp
	pushq     0         # r11
	pushq     0         # r10
	pushq     0         # r9
	pushq     0         # r8
	pushq     0         # rdi
	pushq     0         # rsi
	pushq     0         # rcx
	pushq     0         # rdx
	pushq     0         # rax
    subq      rsp,0x18  # 割り込みハンドラと処理を合わせるため
	movq      rax,rsp   # rspを戻り値に設定
	movq      rsp,rcx   # rspを復旧
    sti
	ret

.global     dispatch
.align      16
dispatch:  # void dispatch(uint64_t next_rsp, uint64_t *current_rsp, uint64_t next_rip);
    movq      [rsi],rsp   # rspを保存
    movq      rsp,rdi     # rspを更新
    addq      rsp,0x18    # 割り込みハンドラと処理を合わせるため
# interrupt stack frame
    movq      [rsp + 0x70],0x10   # ss
    movq      rcx,rsp
    addq      rcx,0x50
    movq      [rsp + 0x68],rcx    # rsp
    movq      [rsp + 0x60],0x202  # rflags
    movq      [rsp + 0x58],8      # cs
    movq      [rsp + 0x50],rdx    # rip
# pop general registers
    popq      rax
    popq      rdx
    popq      rcx
    popq      rsi
    popq      rdi
    popq      r8
    popq      r9
    popq      r10
    popq      r11
    popq      rbp
# go to next thread by iretq
    iretq
