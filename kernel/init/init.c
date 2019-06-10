#include <init/initfunc.h>

void init(void)
{
    init_bss();
    init_gdt();
}
