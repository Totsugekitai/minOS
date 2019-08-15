#include <stdint.h>
#include <util/util.h>

extern uint64_t milli_clock;

void uptime(char *output)
{
    sprintf(&milli_clock, output);
}

