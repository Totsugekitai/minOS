#pragma once
#include <stdint.h>
extern void insert_to_reg(uint64_t *addr);
extern uint64_t get_cr3(void);
extern uint64_t get_cr4(void);
extern uint64_t get_efer(void);
extern void generate_gp(void);

struct general_registers {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
};

extern void get_general_registers(struct general_registers *grs);