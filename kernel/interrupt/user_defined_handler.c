#include <stdint.h>
#include <types/boottypes.h>
#include <device/device.h>
#include <graphics/graphics.h>
#include <interrupt/int_handler.h>

uint64_t milli_clock = 0;
uint64_t *end_of_interrupt = (uint64_t *)0xfee000b0;
extern struct pix_format black;
extern struct pix_format white;
extern struct video_info *vinfo_global;

__attribute__((interrupt))
void timer_handler(struct InterruptFrame *frame)
{
    milli_clock += 1;
    *end_of_interrupt = 0;
}

