.intel_syntax       noprefix

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
