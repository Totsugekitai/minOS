.intel_syntax       noprefix

.global     io_cli
io_cli:
    cli
    ret

.global     io_sti
io_sti:
    sti
    ret

.global     io_inb  # uint8_t io_inb(uint8_t port);
io_inb:
    mov     rdx,rdi
    in      al,dx
    ret

.global     io_in32
io_in32:
    mov     rdx,rdi
    in      eax,dx
    ret

.global     io_outb # void io_outb(uint16_t port, uint8_t byte);
io_outb:
    mov     rdx,rdi
    mov     rax,rsi
    out     dx,al
    ret

.global     io_out32 # void io_out32(uint16_t port, uint32_t data);
io_out32:
    mov     rdx,rdi
    mov     rax,rsi
    out     dx,eax
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

.global     init_irq
init_irq:
# PIC1
    mov     al,0x11
    out     0x20,al
    mov     al,0x20
    out     0x21,al
    mov     al,0x4
    out     0x21,al
# PIC2
    mov     al,0x11
    out     0xa0,al
    mov     al,0x28
    out     0xa1,al
    mov     al,0x2
    out     0xa1,al

    mov     al, 0x04
    out     0x21, al
    mov     al, 0x02
    out     0xa1, al
    mov     al,1
    out     0x21,al
    out     0xa1,al
    
    ret
     
.global     send_eoi
send_eoi:
    mov     al,0x20
    out     0x20,al
    ret

.global     set_rflags
set_rflags:   # void set_rflags(uint64_t);
    pushq   rdi
    popfq
    ret
