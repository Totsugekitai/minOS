#pragma once
#include <stdint.h>

struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

// exception handlers
void de_handler(struct InterruptFrame *frame);
void db_handler(struct InterruptFrame *frame);
void nmi_handler(struct InterruptFrame *frame);
void bp_handler(struct InterruptFrame *frame);
void of_handler(struct InterruptFrame *frame);
void br_handler(struct InterruptFrame *frame);
void ud_handler(struct InterruptFrame *frame);
void nm_handler(struct InterruptFrame *frame);
void df_handler(struct InterruptFrame *frame, uint64_t error_code);
void ts_handler(struct InterruptFrame *frame, uint64_t error_code);
void np_handler(struct InterruptFrame *frame, uint64_t error_code);
void ss_handler(struct InterruptFrame *frame, uint64_t error_code);
void gp_handler(struct InterruptFrame *frame, uint64_t error_code);
void pf_handler(struct InterruptFrame *frame, uint64_t error_code);
void mf_handler(struct InterruptFrame *frame);
void ac_handler(struct InterruptFrame *frame, uint64_t error_code);
void mc_handler(struct InterruptFrame *frame);
void xm_handler(struct InterruptFrame *frame);

// user defined handlers
void timer_handler(struct InterruptFrame *frame);
void keyboard_handler(struct InterruptFrame *frame);
void com1_handler(struct InterruptFrame *frame);
void mouse_handler(struct InterruptFrame *frame);
void empty_handler(struct InterruptFrame *frame);
