#include <stdint.h>
#include <acpi/acpi.h>

#include <util/util.h>
#include <device/device.h>

extern uint64_t milli_clock;
extern char output[];

void sleep(int argc, char *argv[])
{
    puts_serial("argc: ");
    putnum_serial(argc);
    puts_serial("\n");

    if (argc == 2) {
        uint64_t start_time = milli_clock;
        uint64_t end_time = start_time + demstr_to_u64(argv[1]);
        while (milli_clock < end_time) {
            asm volatile("hlt");
        }
        sprintf(" ", output);
    } else {
        sprintf("sleep: bad args", output);
    }
}

