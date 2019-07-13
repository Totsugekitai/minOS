.intel_syntax       noprefix
.global             _ch4lvpg

_ch4lvpg:
# CR4.PAE = 1をする
    mov     rax,cr4
    or      rax,
