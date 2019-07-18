#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>

#endif

// struct segm_descriptor gdt;

// void set_segm_descriptor(struct segm_descriptor* sd, int limit, int base,
//         char type, char s_flag, char dpl, char p_flag)
// {
//     /* ビットの切り分けはSDM 3.4.5参照 */
//     unsigned long l_flag = 1 << (21 + 32);
//     unsigned long db_flag = 0 << (22 + 32);
//     unsigned long g_flag = 1 << (23 + 32);
//     type <<= (unsigned long)(8 + 32);
//     s_flag <<= (unsigned long)(12 + 32);
//     dpl <<= (unsigned long)(13 + 32);
//     p_flag <<= (unsigned long)(15 + 32);
//     unsigned long avl = 0 << (20 + 32);
//     unsigned long base_high = (unsigned long)(base >> (24 + 32));
//     unsigned long base_mid = (unsigned long)((base >> 16) & 0x00FF);
//     unsigned long base_low = (unsigned long)(base & 0xFFFF);
//     unsigned long limit_high = (unsigned long)(limit >> 16);
//     unsigned long limit_low = (unsigned long)(limit & 0xFFFF);
// 
//     sd->limit_low = limit_low;
//     sd->base_low = base_low;
//     sd->base_mid = base_mid;
//     sd->type = type;
//     sd->s_flag = s_flag;
//     sd->dpl_field = dpl;
//     sd->p_flag = p_flag;
//     sd->limit_high = limit_high;
//     sd->avl_bit = avl;
//     sd->l_flag = l_flag;
//     sd->db_flag = db_flag;
//     sd->g_flag = g_flag;
//     sd->base_high = base_high;
// 
//     return;
// }

unsigned long make_segment_descriptor(unsigned int type, unsigned int dpl,
        unsigned int null_flag)
{
    unsigned long segment_descriptor = 0;
    /* baseとlimitは無視されるので0でよい */
    // typeはbit43:41に入れる
    segment_descriptor |= (((unsigned long)type) << 41);
    // dplはbit46:45に入れる
    segment_descriptor |= (((unsigned long)dpl) << 45);
    
    // null_flag == 1でnull descriptor
    if (null_flag == 0) {
        // 他のデフォルト設定
        // G,L,P,S = 1、他は0
        segment_descriptor |= 0x00a0900000000000;
    } else {
        // null descriptorなのですべて0
        segment_descriptor = 0x0000000000000000;
    }

    return segment_descriptor;
}

