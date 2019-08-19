#pragma once
#include <stdint.h>
#include <types/mmtypes.h>
#include <types/boottypes.h>

/* segmentation.c */
// void set_segm_descriptor(struct segm_descriptor *sd, int base, int limit,
//         char type, char s_flag, char dpl, char p_flag);
uint64_t make_segment_descriptor(uint32_t type, uint32_t dpl,
        uint32_t null_flag);

extern void load_gdt(uint64_t *base, uint32_t limit);
extern void set_segment_register(uint64_t cs_val);

