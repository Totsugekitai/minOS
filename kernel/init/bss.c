#include <stdint.h>
extern uint32_t *__bss_start, *__bss_end;

void init_bss(void)
{
    uint8_t k, bss_size = (uint8_t)(__bss_end - __bss_start);
    for (k = 0; k < bss_size; k++) {
        __bss_start[k] = 0x00;
    }

    return;
}

