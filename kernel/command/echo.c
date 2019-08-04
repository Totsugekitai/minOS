#include <stdint.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>

extern uint32_t text_x, text_y;

void echo(char *str, struct video_info *vinfo)
{
    putstr(text_x, text_y, white, black, vinfo, str);
}

