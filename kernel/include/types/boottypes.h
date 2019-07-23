#pragma once
#include <stdint.h>
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
