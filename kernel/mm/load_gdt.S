.intel_syntax   noprefix
.global         _start

_start:
    lgdt    rdi         ; 第1引数に与えられたGDTのアドレスをロード
    ret

