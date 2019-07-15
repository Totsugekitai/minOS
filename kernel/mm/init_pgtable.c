#ifndef MY_HEADE_H_
#define MY_HEADE_H_

#include <init/init_pgtable.h>

#endif

void init_pgtable(void) {
    long *pml4[TABLE_SIZE], *pdp[TABLE_SIZE], *pd[PD_NUM][TABLE_SIZE];
    int i, j;
    // ゼロフィル
    for (i = 0; i < TABLE_SIZE; i++) {
        pml4[i] = 0;
        pdp[i] = 0;
        for (j = 0; j < PD_NUM; j++) {
            pd[j][i] = 0;
        }
    }

    // PDのエントリをフォーマット
    long *pg_start = PAGE_START;
    long pg_size = PAGE_SIZE;
    for (i = 0; i < PD_NUM; i++) {
        for (j = 0; j < TABLE_SIZE; j++) {
            pd[i][j] = (pg_start + (long *)PAGE_SIZE | 0x083);
            pg_size += pg_size;
        }
    }

    // PDPのエントリにPDを登録
    // pdp[0]に&pd[0]を、pdp[1]に&pd[1]を...
    for (i = 0; i < PD_NUM; i++) {
        pdp[i] = ((&pd[i] << 12) | 0x003);
    }
    
    // PML4のエントリにPDPを登録
    // pml4[0]に&pdp[0]を登録するだけ
    pml4[0] = &pdp[0];

    init_cr3(&pml4[0]);

    return;
}
