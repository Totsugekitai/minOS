.intel_syntax       noprefix

.global     io_inb  # uint8_t io_inb(uint8_t port);
io_inb:
    mov     rdx,rdi
    in      al,dx
    ret

.global     io_outb # void io_outb(uint16_t port, uint8_t byte);
io_outb:
    mov     rdx,rdi
    mov     rax,rsi
    out     dx,al
    ret

.global     io_rdmsr # uint64_t io_rdmsr(uint32_t msr);
io_rdmsr:
    mov     rcx,rdi
    rdmsr
    shr     rdx,32
    or      rax,rdx
    ret

.global     io_wrmsr # void io_wrmsr(uint32_t edx, uint32_t eax);
io_wrmsr:
    mov     rdx,rdi
    mov     rax,rsi
    wrmsr
    ret
