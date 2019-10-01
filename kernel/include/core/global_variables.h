#pragma once
#include <stdint.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>

extern struct video_info *vinfo_global;
extern struct RSDP *rsdp;
extern struct memory_descriptor *start_mmap;
extern uint64_t mmapsize;
extern uint64_t memdescsize;

extern uint64_t *GDT;

extern uint64_t *PML4;
extern uint64_t *PDP;
extern uint64_t *PD;

extern struct gate_descriptor *IDT;
