#pragma once
#include <types/boottypes.h>

void init_bss(void);
void init_global_variables(struct bootinfo *binfo);
void init_gdt(void);
void init_paging(void);
void init_interrupt(void);
void init_graphics(void);
