.intel_syntax       noprefix

extern vinfo

.global     global_protection
global_protection:
    mov     rdi,vinfo_global    // 呼び出し関数への変数渡し
    call    gp_handler

