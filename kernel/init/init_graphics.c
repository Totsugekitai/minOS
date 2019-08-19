#include <stdint.h>
#include <core/global_variables.h>
#include <graphics/graphics.h>
#include <types/boottypes.h>

void init_graphics(void)
{
    uint32_t x_axis = vinfo_global->x_axis;
    uint32_t y_axis = vinfo_global->y_axis;
    uint32_t ppsl = vinfo_global->ppsl;
    struct pix_format *fb = vinfo_global->fb;
    for (uint32_t i = 0; i < y_axis; i++) {
        for (uint32_t j = 0; j < x_axis; j++) {
            fb[i * ppsl + j] = white;
        }
    }
}
