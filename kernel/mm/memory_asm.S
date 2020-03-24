.intel_syntax   noprefix
.code64

.global load_cr0
load_cr0:
    mov     rax,cr0
    ret

.global store_cr0
store_cr0:
    mov     rax,rdi
    mov     cr0,rax
    ret

.global             load_pgtable
load_pgtable:       # load_pgtable(long *pml4_addr);
# CR3にPML4のアドレスを格納する
    mov     rcx,rdi     # PML4のアドレスをrcxに格納
    or      rcx,0x18    # cr3のPWTとPCDを立てる
    mov     cr3,rcx     # cr3に反映
    ret

.global         load_gdt
load_gdt:
    mov     rax,rdi     # 第一引数はbase address
    mov     rcx,rsi     # 第二引数はlimit
    mov     rdx,0x10
    mov     [rdx],cx
    mov     [rdx + 2],rax
    lgdt    [rdx]
    ret

.global             set_segment_register
set_segment_register:
    cli             # まずは割り込みを禁止

    mov     ax,0x0 # ax = 0x0

# segment register(CS以外)をnull selectorに設定
    mov     ds,ax
    mov     es,ax
    mov     ss,ax
    mov     gs,ax
    mov     fs,ax

# CSの設定にiretq命令を使う
# スタックにSS,RSP,RFLAGS,CS,RIPの順に設定したい値を入れていく
    mov     rcx,rsp
    push    0       # ss = 0に設定
    push    rcx     # rspはそのまま
    pushfq          # RFLAGSはそのまま
    push    rdi     # cs = rdi(第一引数)に設定(権限のみ見る)
    lea     rcx,main_label
    push    rcx  # rip = main_labelのアドレス
    hlt
    iretq           # pushした値をそれぞれに書き込み、main_labelにジャンプ

.global             main_label
main_label:
    sti
    call    main_routine
