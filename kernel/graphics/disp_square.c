#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <types/typedef.h>

void disp_dot(unsigned int x, unsigned int y,
        unsigned int color, struct video_info *vinfo)
{
    struct pix_format *fb = vinfo->fb;
    unsigned int x_axis = vinfo->x_axis;
    unsigned int y_axis = vinfo->y_axis;
    unsigned int ppsl = vinfo->ppsl;

    if (!(x < x_axis) || !(y < y_axis)) {
        return;
    }

    struct pix_format *pix;
    pix->bl = (color << 3 * 8) >> 3 * 8;
    pix->gr = (color << 2 * 8) >> 3 * 8;
    pix->re = color >> 3 * 8;
    pix->_reserved = 0x00;

    fb[x + y * ppsl] = *pix;

    return;
}

void disp_square(unsigned int ul_x, unsigned int ul_y, unsigned int color,
        unsigned int x_len, unsigned int y_len, struct video_info *vinfo)
{
    unsigned int i, j;
    unsigned int x_axis = vinfo->x_axis;
    unsigned int y_axis = vinfo->y_axis;

    if (!(ul_x + x_len < x_axis) || !(ul_y + y_len < y_axis)) {
        return;
    }

    for (i = 0; i < y_len; j++) {
        for (j = 0; j < x_len; j++) {
            disp_dot(ul_x + j, ul_y + i, color, vinfo);
        }
    }

    return;
}
