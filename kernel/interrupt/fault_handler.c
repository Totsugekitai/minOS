#pragma once
#include <types/boottypes.h>
#include <graphics/graphics.h>

void gp_handler(struct video_info *vinfo) {
    putstr(600, 70, black, white, vinfo, "#GP");
}
