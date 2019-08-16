#include <stdint.h>
#include <util/util.h>

extern uint64_t milli_clock;

void uptime(char *output)
{
    char clockstr[19];
    u64_to_hexstr(clockstr, milli_clock);
    sprintf(clockstr, output);
}

