.intel_syntax       noprefix
.global             change_4lv_pg

change_4lv_pg:
# CR0.PG = 0をやる(いったん落としておかないとバグる)
    mov     eax,cr0
    and     eax,0x7fffffff
    mov     cr0,eax

# ここから
# CR4.PAE = 1をやる
    ; mov     eax,cr4
    ; or      eax,0x20
    ; mov     cr4,eax
# IA32_EFER.LME = 1をやる
    ; mov     ecx,0xc0000080      # ecx = 0xc0000080にセットすると
    ; rdmsr                       # eaxにIA32_EFERが読み込まれる
    ; or      eax,0x100           # eaxの8bit目(LMEフラグ)を立てる
    ; wrmsr                       # eaxをIA32_EFERに書き込む
# ここまでは既に行われていたので省略

# CR0.PG = 1をやる
    mov     eax,cr0
    or      eax,0x80010000      # eaxの31bit目(PGフラグ)を立てる(ついでにWPフラグも立てておく)
    mov     cr0,eax             # ここでページングが有効になる
    ret
