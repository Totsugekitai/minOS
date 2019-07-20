#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>
#include <types/boottypes.h>

#endif

/* segmentation.c */
// void set_segm_descriptor(struct segm_descriptor *sd, int base, int limit,
//         char type, char s_flag, char dpl, char p_flag);
unsigned long make_segment_descriptor(unsigned int type, unsigned int dpl,
        unsigned int null_flag);

extern void load_gdt(unsigned long *base, unsigned int limit);
extern void set_segment_register(unsigned long cs_val,
        struct video_info *vinfo);

