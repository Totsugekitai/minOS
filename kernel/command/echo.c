#include <stdint.h>
#include <util/util.h>
#include <device/device.h>

extern char output[];

void echo(int argc, char *argv[])
{
    puts_serial("argc: ");
    putnum_serial(argc);
    puts_serial("\n");
    puts_serial("argv[1]: ");
    puts_serial(argv[1]);
    puts_serial("\n");

    if (argc != 2) {
        sprintf("echo: bad args", output);
    } else {
        sprintf(argv[1], output);
    }
}

