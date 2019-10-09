.intel_syntax       noprefix

.global stack_init
stack_init:     # uint64_t stack_init(uint64_t *stack_bottom, void(*func)(int, char**));
    mov     rcx,rsp
    mov     rsp,rdi
    pushq   0x10
    pushq   rdi
    pushq   0x202
    pushq   8
    pushq   rsi
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    pushq   0
    mov     rax,rsp
    mov     rsp,rcx
    ret

.global save_and_dispatch
save_and_dispatch:     # void save_and_dispatch(uint64_t *current_rsp, uint64_t next_rsp);
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
# current_rspを保存し、next_rspをロードする
    mov     [rdi],rsp
    mov     rsp,rsi
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
    iretq

.global dispatch
dispatch:           # void dispatch(uint64_t next_rsp);
# next_rspをロードする
    mov     rsp,rdi
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
    iretq

.global save_and_dispatch2
save_and_dispatch2:     # void save_and_dispatch2(uint64_t *current_rsp, uint64_t next_rsp);
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
# current_rspを保存し、next_rspをロードする
    mov     [rdi],rsp
    mov     rsp,rsi
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
    hlt
    iretq
