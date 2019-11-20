#include <stdint.h>
#include <util/util.h>
#include <graphics/graphics.h>
#include <device/device.h>

extern uint64_t __heap_start, __heap_end;

void memory_test(void)
{
    uint64_t *t = &__heap_start;
    uint64_t *i;
    for (i = t; i < &__heap_end; i += 0x1000) {
        putnum_serial((uint64_t)i);
        puts_serial(": ");
        putnum_serial(*i);
        puts_serial("\n");
    }
}