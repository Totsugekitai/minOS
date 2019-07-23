#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#ifndef MY_HEADER_H_
#define MY_HEADER_H_

#include <types/boottypes.h>
#include <graphics/fonts_bin.h>
#include <util/util.h>

#endif


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

void putnum(unsigned int ul_x, unsigned int ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, unsigned long num)
{
    unsigned long i;
    unsigned long numarr[16];
    char numchararr[19];
    for (i = 0; i < 16; i++) {
        numarr[i] = (num & (pow(2, 0 + i * 4) + pow(2, 1 + i * 4) + pow(2, 2 + i * 4) + pow(2, 3 + i * 4))) >> i * 4;
    }
    numchararr[0] = '0';
    numchararr[1] = 'x';
    numchararr[18] = '\0';
    for (i = 0; i < 16; i++)
    {
        if (numarr[i] == 0x0)
        {
            numchararr[17 - i] = '0';
        }
        if (numarr[i] == 0x1)
        {
            numchararr[17 - i] = '1';
        }
        if (numarr[i] == 0x2)
        {
            numchararr[17 - i] = '2';
        }
        if (numarr[i] == 0x3)
        {
            numchararr[17 - i] = '3';
        }
        if (numarr[i] == 0x4)
        {
            numchararr[17 - i] = '4';
        }
        if (numarr[i] == 0x5)
        {
            numchararr[17 - i] = '5';
        }
        if (numarr[i] == 0x6)
        {
            numchararr[17 - i] = '6';
        }
        if (numarr[i] == 0x7)
        {
            numchararr[17 - i] = '7';
        }
        if (numarr[i] == 0x8)
        {
            numchararr[17 - i] = '8';
        }
        if (numarr[i] == 0x9)
        {
            numchararr[17 - i] = '9';
        }
        if (numarr[i] == 0xa)
        {
            numchararr[17 - i] = 'a';
        }
        if (numarr[i] == 0xb)
        {
            numchararr[17 - i] = 'b';
        }
        if (numarr[i] == 0xc)
        {
            numchararr[17 - i] = 'c';
        }
        if (numarr[i] == 0xd)
        {
            numchararr[17 - i] = 'd';
        }
        if (numarr[i] == 0xe)
        {
            numchararr[17 - i] = 'e';
        }
        if (numarr[i] == 0xf)
        {
            numchararr[17 - i] = 'f';
        }
    }
    putstr(ul_x, ul_y, color, bcolor, vinfo, numchararr);
}
