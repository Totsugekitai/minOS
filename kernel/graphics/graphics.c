#include <stdint.h>
#include <types/boottypes.h>
#include <graphics/fonts_bin.h>
#include <util/util.h>

struct pix_format black = {0x00, 0x00, 0x00, 0x00};
struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
struct pix_format green = {0x00, 0xFF, 0x00, 0x00};
struct pix_format blue = {0xFF, 0x00, 0x00, 0x00};

void _draw_dot(uint32_t x, uint32_t y,
        struct pix_format color, struct video_info *vinfo)
{
    struct pix_format *fb = vinfo->fb;
    uint32_t x_axis = vinfo->x_axis;
    uint32_t y_axis = vinfo->y_axis;
    uint32_t ppsl = vinfo->ppsl;

    if (!(x < x_axis) || !(y < y_axis)) {
        return;
    }

    fb[x + y * ppsl] = color;

    return;
}

void draw_square(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        uint32_t x_len, uint32_t y_len, struct video_info *vinfo)
{
    uint32_t i, j;
    uint32_t x_axis = vinfo->x_axis;
    uint32_t y_axis = vinfo->y_axis;

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

void putchar(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c)
{
    char d, c_num = c - 0x20;
    int i, j;

    io_cli();   // 割り込み禁止
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
    io_sti();   // 割り込み許可
}

void putstr(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s)
{
    int i = 0;
    while (s[i] != '\0') {
        putchar(ul_x + i * 8, ul_y, color, bcolor, vinfo, s[i]);
        i++;
    }

    return;
}

void putnum(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, uint64_t num)
{
    uint64_t i;
    uint64_t numarr[16];
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
