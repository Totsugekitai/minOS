#include <types/typedef.h>
#include <core/kernel.h>
#include <graphics/graphics.h>

void start_kernel(struct bootinfo *binfo)
{
    int k, bss_size = (int)(__bss_end - __bss_start);
    for (k = 0; k < bss_size; k++) {
        __bss_start[k] = 0;
    }

    struct video_info vinfo = binfo->vinfo;
    unsigned int i, j;

    // フレームバッファの初期化作業
    unsigned int x_axis = vinfo.x_axis;
    unsigned int y_axis = vinfo.y_axis;
    unsigned int ppsl = vinfo.ppsl;
    struct pix_format *fb = vinfo.fb;
    struct pix_format white = { 0xFF, 0xFF, 0xFF, 0x00 };
    for (i = 0; i < y_axis; i++) {
        for (j = 0; j < x_axis; j++) {
            fb[i * ppsl + j] = white;
        }
    }

    struct pix_format red   = { 0x00, 0x00, 0xFF, 0x00 };
    struct pix_format green = { 0x00, 0xFF, 0x00, 0x00 };
    struct pix_format blue  = { 0xFF, 0x00, 0x00, 0x00 };

    draw_square(30, 30, red, 300, 200, &vinfo);
    draw_square(60, 60, green, 300, 200, &vinfo);
    draw_square(90, 90, blue, 300, 200, &vinfo);

    struct pix_format black = { 0x00, 0x00, 0x00, 0x00 };

    putchar(100, 100, white, black, &vinfo, 'A');
    putchar(108, 100, white, black, &vinfo, 'B');
    putchar(116, 100, white, black, &vinfo, 'C');

    putstr(100, 132, white, blue, &vinfo,
            "minOS - A Minimal Operating System.");

    while (1) {}

    return;
}
