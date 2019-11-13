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

.global     init_stack2
.align      16
init_stack2:          # uint64_t init_stack2(uint64_t stack_bottom, uint64_t rip);
    cli
	  mov     rcx,rsp   # save rsp at rcx
	  mov     rsp,rdi   # move rip to rsp
# push general registers to stack
    push    rsi       # push rip(second argument) to stack bottom
                      # because function switch_context requires return rip
    push    0         # rbp
    push    0         # r11
    push    0         # r10
    push    0         # r9
    push    0         # r8
    push    0         # rdi
    push    0         # rsi
    push    0         # rcx
    push    0         # rdx
    push    0         # rax
    mov     rax,rsp   # set current rsp to return value
    mov     rsp,rcx   # load rsp from rcx
    sti
    ret

.global     switch_context
.align      16
switch_context:   # void switch_context(uint64_t *current_rsp, uint64_t next_rsp);
# push current task's general registers to stack
    push    rbp
    push    r11
    push    r10
    push    r9
    push    r8
    push    rdi
    push    rsi
    push    rdx
    push    rcx
    push    rax
# switch rsp
    mov     [rdi],rsp
    mov     rsp,rsi
# pop next task's general registers from stack
    pop     rax
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     r8
    pop     r9
    pop     r10
    pop     r11
    pop     rbp
# return to next task
    ret
