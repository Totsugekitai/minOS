#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>

#endif

/* segmentation.c */
// void set_segm_descriptor(struct segm_descriptor *sd, int base, int limit,
//         char type, char s_flag, char dpl, char p_flag);
unsigned long make_segment_descriptor(unsigned int type, unsigned int dpl,
        unsigned int null_flag);

