#include "kernel.h"

void start_kernel(struct bootinfo_t *binfo)
{
    struct video_info_t vinfo = binfo->vinfo;
    unsigned long long *fbptr = (unsigned long long *)vinfo.fb;
    unsigned long long fb_size = vinfo.fb_size;
    unsigned int x_axis = vinfo.x_axis;
    unsigned int y_axis = vinfo.y_axis;
    unsigned int ppsl = vinfo.ppsl;

    unsigned long long i, j;
    for (i = 0; i < y_axis; i++) {
        for (j = 0; j < x_axis; j++) {
            fbptr[i * ppsl + j] = 0x00FF000000FF0000;
        }
    }

    while (1) {}

    return;
}
