#include "kernel.h"

void start_kernel(struct bootinfo_t *binfo)
{
        struct video_info_t vinfo = binfo->vinfo;
        unsigned long long *fbptr = (unsigned long long *)vinfo.fb;
        unsigned long long fbsize = vinfo.fb_size;

        unsigned long long i;
        for (i = 0; i < fbsize; i++) {
                fbptr[i] = 0xffffffff;
        }

        while (1) {}

        return;
}
