.intel_syntax   noprefix
.global         load_gdt

load_gdt:
    mov     rax,rdi     # 第一引数はbase address
    mov     rcx,rsi     # 第二引数はlimit
    mov     [r8],rcx
    mov     [r8 + 2],rax
    lgdt    [r8]
    ret

