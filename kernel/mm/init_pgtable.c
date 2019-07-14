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
    long *k = PAGE_START;
    for (i = 0; i < PD_NUM; i++) {
        for (j = 0; j < TABLE_SIZE; j++) {
            // bit[35:21]に物理アドレスを登録
            pd[i][j] ||= (k << 21);
            k += (long *)PAGE_SIZE;
            // その他のbitを設定
            pd[i][j] ||= ;
        }
    }

    // PDPのエントリにPDを登録
    for (i = 0; i < PD_NUM; i++) {
        pdp[i] = &pd[i];
    }
    // 残りは0

    return;
}
