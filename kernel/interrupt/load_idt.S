.intel_syntax       noprefix

.global     load_idt
load_idt:
    mov     rax,rdi     # 第一引数はbase address
    mov     rcx,rsi     # 第二引数はlimit
    mov     rdx,0x10
    mov     [rdx],cx
    mov     [rdx + 2],rax
    lidt    [rdx]
    ret
