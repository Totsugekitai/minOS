#include <types/typedef.h>
#include <core/kernel.h>
#include <graphics/graphics.h>

void start_kernel(struct bootinfo *binfo)
{
    struct video_info *vinfo = binfo->vinfo;

    disp_square(50, 50, 0xff0000, 50, 50, vinfo);
    disp_square(30, 30, 0x008000, 50, 50, vinfo);

    while (1) {}

    return;
}
