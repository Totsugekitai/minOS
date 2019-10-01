.intel_syntax   noprefix

.global load_cr0
load_cr0:
    mov     rax,cr0
    ret

.global store_cr0
store_cr0:
    mov     rax,rdi
    mov     cr0,rax
    ret

