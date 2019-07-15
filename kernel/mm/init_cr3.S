.intel_syntax           noprefix
.global                 _init_cr3

_init_cr3:
    mov     rax,rdi         # 第一引数(PML4テーブルの先頭アドレス)をraxにコピー

# CR3へ格納するビット列を整える
    shl     rax,12          # PML4の先頭アドレスを12bit左へシフト
    and     rax,0xffffff000 # PML4テーブルの先頭アドレス以外の部分は0にする
    mov     cr3,rax         # raxをCR3にセット
    ret
