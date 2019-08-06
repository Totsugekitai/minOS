.intel_syntax       noprefix

.global     disabling_PIC
disabling_PIC:
    mov     al,0xff
    out     0xa1,al
    out     0x21,al
    ret
