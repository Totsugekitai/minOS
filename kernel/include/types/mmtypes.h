/* segmentation */

struct segm_descriptor {
    unsigned long limit_low : 16;
    unsigned long base_low : 16;
    unsigned long base_mid : 8;
    unsigned long type : 4;
    unsigned long s_flag : 1;
    unsigned long dpl_field : 2;
    unsigned long p_flag : 1;
    unsigned long limit_high : 4;
    unsigned long avl_bit : 1;
    unsigned long l_flag : 1;
    unsigned long db_flag : 1;
    unsigned long g_flag : 1;
    unsigned long base_high : 8;
};

/* paging */


