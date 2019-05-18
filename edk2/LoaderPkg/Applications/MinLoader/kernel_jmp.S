.global  kernel_jmp

kernel_jmp:
    movq    %rcx,%rbp
    movq    %rdx,%rsp           // stack_baseをスタックポインタに設定
    jmp     *%rbp               // kernel_startにジャンプ
