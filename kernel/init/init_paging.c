#include <stdint.h>
#include <core/global_variables.h>
#include <mm/paging.h>

void init_paging(void)
{
    create_pgtable(PML4, PDP, PD);
    load_pgtable(PML4);
}
