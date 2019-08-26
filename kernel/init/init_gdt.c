#include <stdint.h>
#include <mm/segmentation.h>
#include <core/global_variables.h>

void init_gdt(void)
{
    // 空, KERNEL_CS, USER_CSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(5, 3, 0);
    load_gdt(GDT, 0x17);
    // セグメントレジスタの設定をやってmain_routineへとぶ
    set_segment_register(0x8);
}
