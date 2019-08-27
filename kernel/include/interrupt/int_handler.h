#pragma once
#include <stdint.h>

struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

extern void general_protection(void);
extern void page_fault(void);
extern void timer_interrupt(void);
extern void keyboard_interrupt(void);
extern void com1_interrupt(void);
void gp_handler_deluxe(struct InterruptFrame *frame, uint64_t error_code);
void timer_handler(struct InterruptFrame *frame);
void timer_handler_dash(void);
void keyboard_handler(struct InterruptFrame *frame);
void keyboard_handler_dash(void);
void com1_handler(struct InterruptFrame *frame);
void com1_handler_dash(void);

