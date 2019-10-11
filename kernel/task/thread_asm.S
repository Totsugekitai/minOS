.intel_syntax       noprefix

.global stack_init
# スレッドの初期スタックを作成し、rspを適切な位置に進めて戻り値として返す
stack_init:     # uint64_t stack_init(uint64_t *stack_bottom, uint64_t rip);
    movq     rcx,rsp
    movq     rsp,rdi
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
    movq     rax,rsp
    movq     rsp,rcx
    ret

.global save_and_dispatch
save_and_dispatch:     # void save_and_dispatch(uint64_t *current_rsp, uint64_t next_rsp);
# 今のスレッドの汎用レジスタをスタックに退避
    pushq    rbp
    pushq    rax
    pushq    rbx
    pushq    rcx
    pushq    rdx
    pushq    rsi
    pushq    rdi
    pushq    r8
    pushq    r9
    pushq    r10
    pushq    r11
    pushq    r12
    pushq    r13
    pushq    r14
    pushq    r15
# current_rspを保存し、next_rspをロードする
    movq     [rdi],rsp
    movq     rsp,rsi
# 次のスレッドの汎用レジスタをスタックから復帰
    popq     r15
    popq     r14
    popq     r13
    popq     r12
    popq     r11
    popq     r10
    popq     r9
    popq     r8
    popq     rdi
    popq     rsi
    popq     rdx
    popq     rcx
    popq     rbx
    popq     rax
    popq     rbp
    iretq

.global dispatch
dispatch:           # void dispatch(uint64_t next_rsp);
# next_rspをロードする
    movq     rsp,rdi
# 次のスレッドの汎用レジスタをスタックから復帰
    popq     r15
    popq     r14
    popq     r13
    popq     r12
    popq     r11
    popq     r10
    popq     r9
    popq     r8
    popq     rdi
    popq     rsi
    popq     rdx
    popq     rcx
    popq     rbx
    popq     rax
    popq     rbp
    iretq

# デバッグ用
.global save_and_dispatch2
save_and_dispatch2:     # void save_and_dispatch2(uint64_t *current_rsp, uint64_t next_rsp, void (*func)(int, char**));
# 今のスレッドの汎用レジスタをスタックに退避
    pushq    rbp
    pushq    rax
    pushq    rbx
    pushq    rcx
    pushq    rdx
    pushq    rsi
    pushq    rdi
    pushq    r8
    pushq    r9
    pushq    r10
    pushq    r11
    pushq    r12
    pushq    r13
    pushq    r14
    pushq    r15
# current_rspを保存し、next_rspをロードする
    movq     [rdi],rsp
    movq     rsp,rsi
# 次のスレッドの汎用レジスタをスタックから復帰
    popq     r15
    popq     r14
    popq     r13
    popq     r12
    popq     r11
    popq     r10
    popq     r9
    popq     r8
    popq     rdi
    popq     rsi
    popq     rax    # rdxは関数呼び出しの引数で使うためここでは用いない
    popq     rcx
    popq     rbx
    popq     rax
    popq     rbp
#  割り込み時に入るスタックの様子がおかしいので修正
    add     rsp,48
    mov     rdi,rsp
    pushq   0
    pushq   rdi
    pushq   0x202
    pushq   8
#    lea     rdx,console
    pushq   rdx
# iretqでタスクスイッチ
    iretq

.global dispatch2
dispatch2:     # void save_and_dispatch2(uint64_t next_rsp, uint64_t rip);
    movq     rsp,rsi
# 次のスレッドの汎用レジスタをスタックから復帰
    popq     r15
    popq     r14
    popq     r13
    popq     r12
    popq     r11
    popq     r10
    popq     r9
    popq     r8
    popq     rdi
    popq     rsi
    popq     rax    # rdxは関数呼び出しの引数で使うためここでは用いない
    popq     rcx
    popq     rbx
    popq     rax
    popq     rbp
#  割り込み時に入るスタックの様子がおかしいので修正
    add     rsp,48
    mov     rdi,rsp
    pushq   0
    pushq   rdi
    pushq   0x202
    pushq   8
#    lea     rdx,console
    pushq   rdx
# iretqでタスクスイッチ
    iretq
