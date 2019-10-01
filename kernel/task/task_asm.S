.intel_syntax   noprefix

.global     save_registers
save_registers:     # void save_registers(uint64_t address);
    mov     [rdi +  0],rbp
    mov     [rdi +  8],rsp
    mov     [rdi + 16],rip
    mov     [rdi + 24],rax
    mov     [rdi + 32],rcx
    mov     [rdi + 40],rdx
    mov     [rdi + 48],rbx
    mov     [rdi + 56],rdi
    mov     [rdi + 64],rsi
    mov     [rdi + 72],r8
    mov     [rdi + 80],r9
    mov     [rdi + 88],r10
    mov     [rdi + 96],r11
    mov     [rdi +104],r12
    mov     [rdi +112],r13
    mov     [rdi +120],r14
    mov     [rdi +128],r15
    popfq   [rdi +136]
    ret

