#pragma once
#include <stdint.h>
extern uint8_t read_kbd_signal(void);
extern uint8_t read_kbd_status(void);
void init_serial(void);
uint32_t serial_received(void);
void wait_serial_input(void);
uint8_t read_serial(void);
uint8_t serial_thr_empty(void);
void write_serial(uint8_t c);
void puts_serial(const char *s);
void putnum_serial(uint64_t num);
uint8_t map_scan_to_ascii_set1(uint8_t scan, uint8_t *shift);
//uint8_t map_scan_to_ascii_set2(uint8_t scan, uint8_t *shift);
uint8_t ps2_received(void);

