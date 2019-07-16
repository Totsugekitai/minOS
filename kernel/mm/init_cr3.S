.intel_syntax           noprefix
.global                 init_cr3

init_cr3:
    mov     rax,rdi         # 第一引数(PML4テーブルの先頭アドレス)をraxにコピー

# CR3へ格納するビット列を整える
    shl     rax,12                  # PML4の先頭アドレスを12bit左へシフト
    mov     cr3,rax                 # raxをCR3にセット
    ret
