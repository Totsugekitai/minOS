#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/boottypes.h>
#include <types/mmtypes.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <mm/paging.h>
#include <graphics/graphics.h>
#include <debug/debug.h>

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

    // デバッグ用(1)
    //unsigned long *addr = (unsigned long *)vinfo.fb;
    //insert_to_reg(addr);
    //while (1) {}

    /* フレームバッファの初期化作業 */
    unsigned int x_axis = vinfo.x_axis;
    unsigned int y_axis = vinfo.y_axis;
    unsigned int ppsl = vinfo.ppsl;
    struct pix_format *fb = vinfo.fb;
    for (i = 0; i < y_axis; i++) {
        for (j = 0; j < x_axis; j++) {
            fb[i * ppsl + j] = white;
        }
    }
    /* 初期の画面描画ここまで */
    putstr(400, 400, black, white, &vinfo,
           "minOS - A Minimal Operating System.");
    putstr(400, 420, black, white, &vinfo,
           "Developer : Totsugekitai(@totsugeki8)");

    // デバッグ用(2)
    //unsigned long *addr = (unsigned long *)vinfo.fb;
    //insert_to_reg(addr);
    //while (1) {}

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
    // デバッグ用(3)
    //unsigned long *addr = (unsigned long *)(vinfo->fb);
    //insert_to_reg(addr);
    //while (1) {}
    
    draw_square(0, 0, red, 50, 50, vinfo);

    // デバッグ用(4)
    //unsigned long *addr = (unsigned long *)(vinfo->fb);
    //insert_to_reg(addr);
    //while (1) {}

    /* ページングの初期化 */
    unsigned long *PML4 = (unsigned long *)0x1000;
    unsigned long *PDP = (unsigned long *)0x2000;
    unsigned long *PD = (unsigned long *)0x10000;
    create_pgtable(PML4, PDP, PD);
    load_pgtable(PML4);

    // デバッグ用(5)
    //unsigned long *addr = (unsigned long *)(vinfo->fb);
    //insert_to_reg(addr);
    //while (1) {}

    draw_square(100, 100, green, 50, 50, vinfo);

    putnum(200, 200, black, white, vinfo, 0x0123456789abcdef);

    putnum(250, 250, black, white, vinfo, get_efer());

    while (1) {}
    
    return;
}

