.intel_syntax       noprefix
.global             load_pgtable

load_pgtable:       # load_pgtable(long *pml4_addr);
# CR0.PG = 0をやる(いったん落としておかないとバグる)
    mov     rax,cr0
    and     rax,0x7fffffff
    mov     cr0,rax

# CR3にPML4のアドレスを格納する
    mov     rcx,rdi     # PML4のアドレスをrcxに格納
    or      rcx,0x18    # cr3の下位3bitの調整
    mov     cr3,rcx     # cr3に反映

# CR0.PG = 1をやる
    mov     rax,cr0
    or      eax,0x80000000  # eaxのbit31(PG)を立てる
    mov     cr0,rax     # ここでページングが有効になる

# IA32_EFER.LMAが落ちるので上げておく
    mov     ecx,0xc0000080
    rdmsr
    or      eax,0x400
    wrmsr

    ret

