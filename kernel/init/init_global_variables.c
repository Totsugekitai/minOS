#include <stdint.h>
#include <core/global_variables.h>
#include <types/boottypes.h>

void init_global_variables(struct bootinfo *binfo)
{
    rsdp = binfo->rsdp;
    vinfo_global = &binfo->vinfo;
    start_mmap = binfo->memory_descriptor;
    mmapsize = binfo->mmapsize;
    memdescsize = binfo->memdescsize;
}
