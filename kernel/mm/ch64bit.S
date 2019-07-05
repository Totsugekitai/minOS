.intel_syntax   noprefix
.global         _change_64bit

_change_64bit:
# ページング無効化
        mov     ecx,cr0
        and     ecx,0x7FFFFFFF  # 最上位bitを0にする
        mov     cr0,ecx

# PAEを有効にする
        mov     ecx,cr4
        or      ecx,0x00000020  # cr4の6bit目を立てる
        mov     cr4,ecx

# ページングテーブルをロード

# EFER.LMEを立てる
        mov     ecx,0xc0000080  # rdmsrがecxの値をMSRのアドレスとして
                                # 解釈する
        rdmsr
        or      eax,8           # EFERの8bit目のLMEを立てるとよい
        wrmsr                   # wrmsrでeaxの値をMSRの内容として書き込む

# ページング有効化
        mov     ecx,cr0
        or      ecx,0x80000000  # 最上位bitを1にする
        mov     cr0,ecx
        ret

