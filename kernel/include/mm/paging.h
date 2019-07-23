#pragma once
#include <stdint.h>
void create_pgtable(uint64_t *pml4_addr,
        uint64_t *pdp_addr, uint64_t *pd_addr);
extern void load_pgtable(uint64_t *pml4_addr);
