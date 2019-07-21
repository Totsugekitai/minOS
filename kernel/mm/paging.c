#ifndef MY_HEADE_H_
#define MY_HEADE_H_

#define PML4_NUM        1
#define PD_NUM          16
#define TABLE_SIZE      512
#define PAGE_SIZE_2M    (long)0x00200000 // 2MiB
#define PAGE_SIZE_1G    512 * PAGE_SIZE_2M // 1GiB

#endif

long pml4[TABLE_SIZE], pdp[TABLE_SIZE], pd[PD_NUM][TABLE_SIZE];

long *create_pgtable(void) {
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
    for (i = 0; i < PD_NUM; i++) {
        for (j = 0; j < TABLE_SIZE; j++) {
            pd[i][j] = ((i * PAGE_SIZE_1G + j * PAGE_SIZE_2M) | 0x083);
        }
    }

    // PDPのエントリにPDを登録
    // pdp[0]に&pd[0]を、pdp[1]に&pd[1]を...
    for (i = 0; i < PD_NUM; i++) {
        pdp[i] = (((long)(&pd[i]) << 12) | 0x003);
    }
    
    // PML4のエントリにPDPを登録
    // pml4[0]に&pdp[0]を登録するだけ
    pml4[0] = ((long)(&pdp[0]) | 0x003);

    return pml4;
}
