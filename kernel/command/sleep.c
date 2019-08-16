#include <stdint.h>
#include <acpi/acpi.h>

#include <util/util.h>
#include <device/device.h>

extern uint64_t milli_clock;

void sleep(uint64_t time)
{
    uint64_t start_time = milli_clock;
    uint64_t end_time = start_time + time;
    while (milli_clock < end_time) {
        asm volatile("hlt");
    }
}

