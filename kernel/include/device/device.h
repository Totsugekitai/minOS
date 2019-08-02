#pragma once
#include <stdint.h>
extern uint8_t read_kbd_signal(void);
char map_scan_to_ascii(uint64_t scan, uint8_t shift);
