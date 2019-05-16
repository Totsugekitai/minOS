#include "../boot/minloader.h"

void start_kernel(void *t __attribute__((unused)), bootinfo_t *binfo)
{
        video_info_t *vinfo = binfo->vinfo;
        void *fbptr = vinfo->fb;
        int fbsize = (int)vinfo->fb_size;

        int i;
        for (i = 0; i < fbsize; i++) {
                fbptr[i] = 0xffffff;
        }

        for (;;) __asm__ volatile ("hlt");
}
