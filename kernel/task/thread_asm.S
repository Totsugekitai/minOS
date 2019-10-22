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

.global change_rsp
change_rsp:   # void change_rsp(uint64_t next_rsp);
    movq    rsp,rdi
    ret

.global   init_stack2
init_stack2:  # uint64_t init_stack2(uint64_t *stack_bottom, uint64_t rip);
    movq      rcx,rsp   # スタックポインタを保存
	  movq      rsp,rdi   # スタックポインタを移動
	  pushq     0x10      # ss
	  pushq     rdi       # rsp
	  pushq     0x202     # rflags
	  pushq     8         # cs
	  pushq     rsi       # rip
	  pushq     0         # rbp
	  pushq     0         # rax
	  pushq     0         # r11
	  pushq     0         # r10
	  pushq     0         # r9
	  pushq     0         # r8
	  pushq     0         # rdi
	  pushq     0         # rsi
	  pushq     0         # rdx
	  pushq     0         # rcx
    # sub       rsp,0x118
    movq      rsi,0xdeadbeef
    movq      xmm0,rsi
    movaps    XMMWORD PTR [rdi-0x60],xmm0       # xmm15
    movaps    XMMWORD PTR [rdi-0x70],xmm0       # xmm14
    movaps    XMMWORD PTR [rdi-0x80],xmm0       # xmm13
    movaps    XMMWORD PTR [rdi-0x90],xmm0       # xmm12
    movaps    XMMWORD PTR [rdi-0xa0],xmm0       # xmm11
    movaps    XMMWORD PTR [rdi-0xb0],xmm0       # xmm10
    movaps    XMMWORD PTR [rdi-0xc0],xmm0       # xmm9
    movaps    XMMWORD PTR [rdi-0xd0],xmm0       # xmm8
    movaps    XMMWORD PTR [rdi-0xe0],xmm0       # xmm7
    movaps    XMMWORD PTR [rdi-0xf0],xmm0       # xmm6
    movaps    XMMWORD PTR [rdi-0x100],xmm0      # xmm5
    movaps    XMMWORD PTR [rdi-0x110],xmm0      # xmm4
    movaps    XMMWORD PTR [rdi-0x120],xmm0      # xmm3
    movaps    XMMWORD PTR [rdi-0x130],xmm0      # xmm2
    movaps    XMMWORD PTR [rdi-0x140],xmm0      # xmm1
    movaps    XMMWORD PTR [rdi-0x150],xmm0      # xmm0
	  movq      rax,rsp   # スタックポインタを戻り値に設定
	  movq      rsp,rcx   # スタックポインタを復旧
	  ret

.global     dispatch3
dispatch3:
    movq      rsp,rdi
    # add       rsp,0x118
    movaps    xmm0,XMMWORD PTR [rsp-0x150]
    movaps    xmm1,XMMWORD PTR [rsp-0x140]
    movaps    xmm2,XMMWORD PTR [rsp-0x130]
    movaps    xmm3,XMMWORD PTR [rsp-0x120]
    movaps    xmm4,XMMWORD PTR [rsp-0x110]
    movaps    xmm5,XMMWORD PTR [rsp-0x100]
    movaps    xmm6,XMMWORD PTR [rsp-0xf0]
    movaps    xmm7,XMMWORD PTR [rsp-0xe0]
    movaps    xmm8,XMMWORD PTR [rsp-0xd0]
    movaps    xmm9,XMMWORD PTR [rsp-0xc0]
    movaps    xmm10,XMMWORD PTR [rsp-0xb0]
    movaps    xmm11,XMMWORD PTR [rsp-0xa0]
    movaps    xmm12,XMMWORD PTR [rsp-0x90]
    movaps    xmm13,XMMWORD PTR [rsp-0x80]
    movaps    xmm14,XMMWORD PTR [rsp-0x70]
    movaps    xmm15,XMMWORD PTR [rsp-0x60]
    popq      rcx
    popq      rdx
    popq      rsi
    popq      rdi
    popq      r8
    popq      r9
    popq      r10
    popq      r11
    popq      rax
    popq      rbp
    iretq
