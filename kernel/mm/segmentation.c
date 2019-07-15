#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/mmtypes.h>

#endif

struct segm_selector ret_segm_selector(short index, char ti_flag, char rpl)
{
    // 初期値はビットがすべて1としておく
    struct segm_selector ss = { 0xFF, 0xFF };
    
    /* エラーハンドリング */
    {
        // -1 < index < 8192 を満たさないなら初期値を返す
        if (index < 0 || 8191 < index) {
            return ss;
        }
        // ti_flag == 0,1 を満たさないなら初期値を返す
        if (ti_flag != 0 && ti_flag != 1) {
            return ss;
        }
        // rpl == 0,1,2,3 を満たさないなら初期値を返す
        if (rpl < 0 || 3 < rpl) {
            return ss;
        }
    }
    
    /* ssにビットを切り分けて入れる。切り分け方はSDM 3.4.2参照 */
    ss.high = (char)(index >> 5);
    ss.low = (char)((index << 11) + (ti_flag << 2) + rpl);

    return ss;
}

void set_segm_descriptor(struct segm_descriptor *sd, int limit, int base,
        char type, char s_flag, char dpl, char p_flag)
{
    /* ビットの切り分けはSDM 3.4.5参照 */
    char l_flag = 1 << 5;
    char d_flag = 0 << 6;
    char g_flag = 1 << 7;
    type <<= 8;
    s_flag <<= 12;
    dpl <<= 13;
    p_flag <<= 14;
    char avl = 0 << 20;
    char base_high = (char)(base >> 24);
    char base_mid = (char)((base >> 16) & 0x00FF);
    short base_low = (short)(base & 0xFFFF);
    char limit_high = (char)(limit >> 16);
    short limit_low = (short)(limit & 0xFFFF);

    sd->low_low = limit_low;
    sd->low_high = base_low;
    sd->high_low = p_flag + dpl + s_flag + type + base_mid;
    sd->high_high = base_high + g_flag + d_flag + l_flag + avl + limit_high;

    return;
}

