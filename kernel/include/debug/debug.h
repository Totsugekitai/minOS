#pragma once
#include <stdint.h>
extern void insert_to_reg(uint64_t *addr);
extern uint64_t get_cr3(void);
extern uint64_t get_cr4(void);
extern uint64_t get_efer(void);
