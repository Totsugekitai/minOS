#pragma once
#include <stdint.h>
extern uint8_t read_kbd_signal(void);
void init_serial(void);
uint32_t serial_received(void);
uint8_t read_serial(void);
char map_scan_to_ascii(uint64_t scan, uint8_t shift);
