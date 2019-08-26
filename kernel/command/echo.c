#include <stdint.h>
#include <util/util.h>

extern uint32_t text_x, text_y;

void echo(char *arg, char *output)
{
    sprintf(arg, output);
}

