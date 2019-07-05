.intel_syntax   noprefix
.global         _load_gdt

_load_gdt:
    lgdt    rdi         ; 第1引数に与えられたGDTのアドレスをロード
    ret

