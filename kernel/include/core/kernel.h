#pragma once
#include <stdint.h>
/* types */
struct pix_format {
    uint8_t bl;
    uint8_t gr;
    uint8_t re;
    uint8_t _reserved;
};

struct video_info {
    struct pix_format *fb;
    uint64_t fb_size;
    uint32_t x_axis;
    uint32_t y_axis;
    uint32_t ppsl;
};

struct bootinfo {
    struct video_info vinfo;
};

/* graphics */
// ulはupper leftの略
void draw_square(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        uint32_t x_len, uint32_t y_len, struct video_info *vinfo);
void putchar(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c);
void putstr(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s);

