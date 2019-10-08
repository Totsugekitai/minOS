.intel_syntax       noprefix

.global save_and_jump
save_and_jump:    # void save_and_jump(uint64_t *task_address);
    push    rbp
	  push    rax
	  push    rbx
	  push    rcx
	  push    rdx
	  push    rsi
	  push    rdi
	  push    r8
	  push    r9
	  push    r10
	  push    r11
	  push    r12
	  push    r13
	  push    r14
	  push    r15
    push    rip
    ret

.global save_and_dispatch
save_and_dispatch:     # void save_and_dispatch(uint64_t *current_rsp, uint64_t *next_rsp);
# 今のスレッドの汎用レジスタをスタックに退避
    push    rbp
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rsi
    push    rdi
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
# current_rspとnext_rspを入れ替える
    mov     [rdi],rsp
    mov     rsp,[rsi]
# 次のスレッドの汎用レジスタをスタックから復帰
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rdi
    pop     rsi
    pop     rdx
    pop     rcx
    pop     rbx
    pop     rax
    pop     rbp

    ret

.global stack_init_and_jump
stack_init_and_jump:     # void stacked_registers_init(uint64_t *stack_bottom, uint64_t *func_addr);
    movq    [rdi - 0],0         # rbp
    movq    [rdi - 8],0         # rax
    movq    [rdi - 16],0        # rbx
    movq    [rdi - 24],0        # rcx
    movq    [rdi - 32],0        # rdx
    movq    [rdi - 40],0        # rsi
    movq    [rdi - 48],0        # rdi
    movq    [rdi - 56],0        # r8
    movq    [rdi - 64],0        # r9
    movq    [rdi - 72],0        # r10
    movq    [rdi - 80],0        # r11
    movq    [rdi - 88],0        # r12
    movq    [rdi - 96],0        # r13
    movq    [rdi - 104],0       # r14
    movq    [rdi - 112],0       # r15
    movq    [rdi - 120],rsi     # func_addr
    ret                         # ここでジャンプ

