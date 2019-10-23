#pragma once
#include <stdint.h>

struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void gp_handler(struct InterruptFrame *frame, uint64_t error_code);
void pf_handler(struct InterruptFrame *frame, uint64_t error_code);
void timer_handler(struct InterruptFrame *frame);
void keyboard_handler(struct InterruptFrame *frame);
void com1_handler(struct InterruptFrame *frame);
void mouse_handler(struct InterruptFrame *frame);
void empty_handler(struct InterruptFrame *frame);
