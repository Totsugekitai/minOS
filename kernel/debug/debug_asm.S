.intel_syntax       noprefix

.global     insert_to_reg
insert_to_reg:
    mov     rcx,rdi
    ret

.global     get_cr3
get_cr3:
    mov     rax,cr3
    ret

.global     get_cr4
get_cr4:
    mov     rax,cr4
    ret

.global     get_efer
get_efer:
    mov     ecx,0xc0000080
    rdmsr
    ret

.global     generate_gp
generate_gp:
#    mov     rcx,cr4
#    and     cx,0xffdf
#    mov     cr4,rcx
#    ret
    int     13
    ret

.global     get_general_registers
get_general_registers:  # void get_general_registers(struct general_registers *grs);
    mov     [rdi + 0x00],rax
    mov     [rdi + 0x08],rbx
    mov     [rdi + 0x10],rcx
    mov     [rdi + 0x18],rdx
    mov     [rdi + 0x20],rsi
    mov     [rdi + 0x28],rdi
    mov     [rdi + 0x30],rsp
    mov     [rdi + 0x38],rbp
    mov     [rdi + 0x40],r8
    mov     [rdi + 0x48],r9
    mov     [rdi + 0x50],r10
    mov     [rdi + 0x58],r11
    mov     [rdi + 0x60],r12
    mov     [rdi + 0x68],r13
    mov     [rdi + 0x70],r14
    mov     [rdi + 0x78],r15
    ret
