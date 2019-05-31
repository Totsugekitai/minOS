#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <types/typedef.h>
#include <graphics/fonts_bin.h>

void _draw_dot(unsigned int x, unsigned int y,
        struct pix_format color, struct video_info *vinfo)
{
    struct pix_format *fb = vinfo->fb;
    unsigned int x_axis = vinfo->x_axis;
    unsigned int y_axis = vinfo->y_axis;
    unsigned int ppsl = vinfo->ppsl;

    if (!(x < x_axis) || !(y < y_axis)) {
        return;
    }

    fb[x + y * ppsl] = color;

    return;
}

void draw_square(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        unsigned int x_len, unsigned int y_len, struct video_info *vinfo)
{
    unsigned int i, j;
    unsigned int x_axis = vinfo->x_axis;
    unsigned int y_axis = vinfo->y_axis;

    if (!(ul_x + x_len < x_axis) || !(ul_y + y_len < y_axis)) {
        return;
    }

    for (i = 0; i < y_len; i++) {
        for (j = 0; j < x_len; j++) {
            _draw_dot(ul_x + j, ul_y + i, color, vinfo);
        }
    }

    return;
}

void putchar(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c)
{
    char d, c_num = c - 0x20;
    int i, j;

    // 背景を塗る
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 8; j++) {
            _draw_dot(ul_x + j, ul_y + i, bcolor, vinfo);
        }
    }

    // 文字を書く
    for (i = 0; i < 16; i++) {
        d = fonts[c_num * 16 + i];
        if ((d & 0x80) != 0) { _draw_dot(ul_x + 0, ul_y + i, color, vinfo); }
        if ((d & 0x40) != 0) { _draw_dot(ul_x + 1, ul_y + i, color, vinfo); }
        if ((d & 0x20) != 0) { _draw_dot(ul_x + 2, ul_y + i, color, vinfo); }
        if ((d & 0x10) != 0) { _draw_dot(ul_x + 3, ul_y + i, color, vinfo); }
        if ((d & 0x08) != 0) { _draw_dot(ul_x + 4, ul_y + i, color, vinfo); }
        if ((d & 0x04) != 0) { _draw_dot(ul_x + 5, ul_y + i, color, vinfo); }
        if ((d & 0x02) != 0) { _draw_dot(ul_x + 6, ul_y + i, color, vinfo); }
        if ((d & 0x01) != 0) { _draw_dot(ul_x + 7, ul_y + i, color, vinfo); }
    }

    return;
}

void putstr(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s)
{
    int i = 0;
    while (s[i] != '\0') {
        putchar(ul_x + i * 8, ul_y, color, bcolor, vinfo, s[i]);
        i++;
    }

    return;
}
