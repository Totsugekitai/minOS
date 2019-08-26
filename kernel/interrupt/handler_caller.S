.intel_syntax       noprefix

.global     general_protection
general_protection:
# まず汎用レジスタをスタックに退避
    hlt
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

# ハンドラ本体を呼び出す
    call    gp_handler

# 汎用レジスタを復帰
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

# 最後は割り込みから復帰
    iretq

.global     page_fault
page_fault:
# まず汎用レジスタをスタックに退避
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

# ハンドラ本体を呼び出す
    call    pf_handler

# 汎用レジスタを復帰
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

# 最後は割り込みから復帰
    iretq

.global     timer_interrupt
timer_interrupt:
# まず汎用レジスタをスタックに退避
    iretq
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

# ハンドラ本体を呼び出す
    call    timer_handler

# 汎用レジスタを復帰
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

# 最後は割り込みから復帰
    iretq

