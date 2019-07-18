.intel_syntax       noprefix

.global     show_cr0
show_cr0:
    mov     rax,cr0
    ret

.global     show_cr4
show_cr4:
    mov     rax,cr4
    ret

.global     show_ia32_efer
show_ia32_efer:
    mov     ecx,0xc0000080      # ecx = 0xc0000080にセットすると
    rdmsr                       # eaxにIA32_EFERが読み込まれる
    ret
