#pragma once
#include <stdint.h>
#include <types/boottypes.h>

/* graphics.c */
void draw_square(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        uint32_t x_len, uint32_t y_len, struct video_info *vinfo);
void putchar(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char c);
void putstr(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, char *s);
void putnum(uint32_t ul_x, uint32_t ul_y, struct pix_format color,
        struct pix_format bcolor, struct video_info *vinfo, uint64_t num);
