.intel_syntax       noprefix
.global             load_pgtable

load_pgtable:       # load_pgtable(long *pml4_addr);
# CR3にPML4のアドレスを格納する
    mov     rcx,rdi     # PML4のアドレスをrcxに格納
    or      rcx,0x18    # cr3のPWTとPCDを立てる
    mov     cr3,rcx     # cr3に反映
    ret

