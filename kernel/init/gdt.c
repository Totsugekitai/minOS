#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <mm/segmentation.h>
#include <types/mmtypes.h>

#endif

#define TABLE_LIMIT         (struct segm_descriptor *)0x9FFFF

void init_gdt(void)
{
    struct segm_descriptor *i;
    // 0x00000 ~ 0x9FFFFがちょうど空いていたのでそこをGDTにする
    for (i = 0x00000; i < TABLE_LIMIT; i++) {
        // とりあえずtype=15,s=0,dpl=0,p=0に設定しておく
        set_segm_descriptor(i, 0, 0, 15, 0, 0, 0);
    }
    return;
}

