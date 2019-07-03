.intel_syntax   noprefix

_change_to_ia_32e:
# ページング無効化
        and     cr0,0x7FFFFFFF  # 最上位bitを0にする

# PAEを有効にする
        mov     ecx,cr4
        or      ecx,0x00000020  # cr4の6bit目を立てる
        mov     cr4,ecx

# EFER.LMEを立てる
        mov     ecx,0xc0000080  # rdmsrがecxの値をMSRのアドレスとして
                                # 解釈する
        rdmsr
        or      eax,8           # EFERの8bit目のLMEを立てるとよい
        wrmsr                   # wrmsrでeaxの値をMSRの内容として書き込む

# ページング有効化
        or      cr0,0x80000000  # 最上位bitを1にする
        ret

