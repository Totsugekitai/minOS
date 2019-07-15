#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>

#endif

/* segmentation.c */
struct segm_selector ret_segm_selector(short index, char ti_flag, char rpl);
void set_segm_descriptor(struct segm_descriptor *sd, int base, int limit,
        char type, char s_flag, char dpl, char p_flag);

