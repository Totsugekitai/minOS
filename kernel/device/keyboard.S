.intel_syntax       noprefix

.global     read_kbd_signal
read_kbd_signal:        # uint8_t read_kbd_signal(void);
    in      al,0x64     # キーボードの信号読み取り
    ret
