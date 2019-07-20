#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/boottypes.h>
#include <types/mmtypes.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <mm/paging.h>
#include <graphics/graphics.h>

#endif

void main_routine(struct video_info *vinfo);

struct pix_format black = {0x00, 0x00, 0x00, 0x00};
struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
struct pix_format green = {0x00, 0xFF, 0x00, 0x00};
struct pix_format blue = {0xFF, 0x00, 0x00, 0x00};

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
    putstr(400, 400, black, white, &vinfo,
           "minOS - A Minimal Operating System.");
    putstr(400, 420, black, white, &vinfo,
           "Developer : Totsugekitai(@totsugeki8)");

    /* 初期の画面描画ここまで */

    /* GDTなどの初期化 */
    init_bss();
    
    unsigned long *GDT = (unsigned long *)0x80;
    
    // 空, KERNEL_CS, USER_CSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(5, 3, 0);
    
    // lgdtでGDTをセットする
    load_gdt(GDT, 0x17);

    // セグメントレジスタの設定
    set_segment_register(0x8, &vinfo);

    return;
}

void main_routine(struct video_info *vinfo)
{
    draw_square(50, 50, red, 50, 50, vinfo);

    /* ページングの初期化 */
    long *PML4 = create_pgtable();
    load_pgtable(PML4);

    draw_square(100, 100, green, 50, 50, vinfo);

    while (1) {}
    
    return;
}

