.intel_syntax       noprefix
.global             load_pgtable

load_pgtable:       # load_pgtable(long *pml4_addr);
# CR0.PG = 0をやる(いったん落としておかないとバグる)
    mov     rax,cr0
    andq     rax,0x7fffffff
    mov     cr0,rax

# CR3にPML4のアドレスを格納する
    mov     rcx,rdi     # PML4のアドレスをrcxに格納
    shl     rcx,12      # PML4のアドレスをCR3にしまうために12bitシフト
    mov     rax,cr3
    and     rax,0x18  # CR3のPWT,PCD以外を全て0にする
    or      eax,ecx     # cr3にpml4のアドレスを入れる
    mov     cr3,rax     # cr3に反映

# CR0.PG = 1をやる
    mov     rax,cr0
    or      rax,0x10000  # eaxのbit31(PG)を立てる(WPも立てる)
    mov     cr0,rax             # ここでページングが有効になる

    ret

