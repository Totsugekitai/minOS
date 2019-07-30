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
    mov     rcx,cr4
    and     cx,0xffdf
    mov     cr4,rcx
    ret
