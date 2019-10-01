.intel_syntax       noprefix
.global             set_segment_register

.code64

set_segment_register:
    cli             # まずは割り込みを禁止

    mov     ax,0x0 # ax = 0x0

# segment register(CS以外)をnull selectorに設定
    mov     ds,ax
    mov     es,ax
    mov     ss,ax
    mov     gs,ax
    mov     fs,ax

# CSを設定する

#    pushfq
#    pop     rcx
#    and     rcx,0xffffffffffffbfff
#    push    rcx
#    popfq

# CSの設定にiretq命令を使う
# スタックにSS,RSP,RFLAGS,CS,RIPの順に設定したい値を入れていく
    mov     rcx,rsp
    push    0     # ss = 0に設定
    push    rcx     # rspはそのまま
    pushfq          # RFLAGSはそのまま
    push    rdi     # cs = rdi(第一引数)に設定(権限のみ見る)
    lea     rcx,main_label
    push    rcx  # rip = main_labelのアドレス
    
    iretq           # pushした値をそれぞれに書き込み、main_labelにジャンプ

.global             main_label
main_label:
    sti
    call    main_routine
