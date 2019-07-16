#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>

#endif

// struct segm_descriptor gdt;

// struct segm_selector ret_segm_selector(short index, char ti_flag, char rpl)
// {
//     // 初期値はビットがすべて1としておく
//     struct segm_selector ss = { 0xFF, 0x01, 0x03 };
    
//     /* エラーハンドリング */
//     {
//         // -1 < index < 8192 を満たさないなら初期値を返す
//         if (index < 0 || 8191 < index) {
//             return ss;
//         }
//         // ti_flag == 0,1 を満たさないなら初期値を返す
//         if (ti_flag != 0 && ti_flag != 1) {
//             return ss;
//         }
//         // rpl == 0,1,2,3 を満たさないなら初期値を返す
//         if (rpl < 0 || 3 < rpl) {
//             return ss;
//         }
//     }
    
//     /* ssにビットを切り分けて入れる。切り分け方はSDM 3.4.2参照 */
//     ss.index = ((index >> 5);
//     ss.low = (char)((index << 11) + (ti_flag << 2) + rpl);

//     return ss;
// }

void set_segm_descriptor(struct segm_descriptor* sd, int limit, int base,
        char type, char s_flag, char dpl, char p_flag)
{
    /* ビットの切り分けはSDM 3.4.5参照 */
    unsigned long l_flag = 1 << (21 + 32);
    unsigned long db_flag = 0 << (22 + 32);
    unsigned long g_flag = 1 << (23 + 32);
    type <<= (unsigned long)(8 + 32);
    s_flag <<= (unsigned long)(12 + 32);
    dpl <<= (unsigned long)(13 + 32);
    p_flag <<= (unsigned long)(15 + 32);
    unsigned long avl = 0 << (20 + 32);
    unsigned long base_high = (unsigned long)(base >> (24 + 32));
    unsigned long base_mid = (unsigned long)((base >> 16) & 0x00FF);
    unsigned long base_low = (unsigned long)(base & 0xFFFF);
    unsigned long limit_high = (unsigned long)(limit >> 16);
    unsigned long limit_low = (unsigned long)(limit & 0xFFFF);

    sd->limit_low = limit_low;
    sd->base_low = base_low;
    sd->base_mid = base_mid;
    sd->type = type;
    sd->s_flag = s_flag;
    sd->dpl_field = dpl;
    sd->p_flag = p_flag;
    sd->limit_high = limit_high;
    sd->avl_bit = avl;
    sd->l_flag = l_flag;
    sd->db_flag = db_flag;
    sd->g_flag = g_flag;
    sd->base_high = base_high;

    return;
}

// void setup_segments(void)
// {

// }
