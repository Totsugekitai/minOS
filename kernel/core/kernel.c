#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/boottypes.h>
#include <types/mmtypes.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <mm/paging.h>
#include <graphics/graphics.h>
extern void load_gdt(unsigned long *, unsigned int);
extern void set_segment_register(unsigned long, struct video_info *);

#endif
void main_routine(struct video_info *vinfo);

void start_kernel(struct bootinfo *binfo)
{
    /* 画面描画 */
    struct video_info vinfo = binfo->vinfo;
    unsigned int i, j;

    /* フレームバッファの初期化作業 */
    {
        unsigned int x_axis = vinfo.x_axis;
        unsigned int y_axis = vinfo.y_axis;
        unsigned int ppsl = vinfo.ppsl;
        struct pix_format *fb = vinfo.fb;
        struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
        for (i = 0; i < y_axis; i++)
        {
            for (j = 0; j < x_axis; j++)
            {
                fb[i * ppsl + j] = white;
            }
        }
    }

    struct pix_format black = {0x00, 0x00, 0x00, 0x00};
    struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
    // struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
    // struct pix_format green = {0x00, 0xFF, 0x00, 0x00};
    // struct pix_format blue = {0xFF, 0x00, 0x00, 0x00};

    putstr(400, 400, black, white, &vinfo,
           "minOS - A Minimal Operating System.");
    putstr(400, 420, black, white, &vinfo,
           "Developer : Totsugekitai(@totsugeki8)");

    /* 初期の画面描画ここまで */

    /* GDTなどの初期化 */

    init_bss();
    // // GDTは0x00000000~0x0009FFFF
    // struct segm_descriptor *gdt = 0x00000000; // ヌル、カーネル、ユーザの3つを用意する
    // // 0個目のdescriptorはヌル
    // set_segm_descriptor(gdt + 0, 0x00000000, 0x00000000, 0, 0, 0, 0);
    // // 1つ目のdescriptorはカーネル用
    // set_segm_descriptor(gdt + 1, 0x00000000, 0xffffffff, 0, 0, 0, 1);
    // // 2つ目のdescriptorはユーザ用
    // set_segm_descriptor(gdt + 2, 0x00000000, 0xffffffff, 3, 0, 3, 1);
    
    unsigned long *GDT = (unsigned long *)0x0000000000000080;
    
    // 空, KERNEL_CS, USER_CSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(5, 3, 0);
    
    // lgdtでGDTをセットする
    load_gdt(GDT, 0x17);

    // セグメントレジスタの設定
    set_segment_register(0x0000000000000008, &vinfo);

    /* ページング初期化 */
    
    // init_pgtable(); // ページングテーブルの初期化
    // change_4lv_pg(); // 4-levelページングに移行

    while (1) {}

    return;
}

void main_routine(struct video_info *vinfo)
{
    struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
    draw_square(50, 50, red, 50, 50, vinfo);

    while (1) {}
    
    return;
}
