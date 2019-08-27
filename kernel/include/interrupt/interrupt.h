#pragma once
#include <stdint.h>

#define PIC0_ICW1   0x20
#define PIC0_OCW2   0x20
#define PIC0_IMR    0x21
#define PIC0_ICW2   0x21
#define PIC0_ICW3   0x21
#define PIC0_ICW4   0x21
#define PIC1_ICW1   0xa0
#define PIC1_OCW2   0xa0
#define PIC1_IMR    0xa1
#define PIC1_ICW2   0xa1
#define PIC1_ICW3   0xa1
#define PIC1_ICW4   0xa1


struct gate_descriptor {
    uint32_t low;
    uint32_t mid;
    uint64_t high;
};

struct gate_descriptor make_gate_descriptor(uint64_t offset, uint32_t dpl,
        uint32_t i_or_g, uint32_t ist);

void init_pic(void);

extern void load_idt(uint64_t base, uint64_t limit);
