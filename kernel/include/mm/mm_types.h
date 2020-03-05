#pragma once
#include <stdint.h>

struct vm_area_struct {
    uint64_t *vm_start;     // start address
    uint64_t *vm_end;       // the first byte after end address

    // vm_next: next vm_area_struct pointer
    // vm_prev: previous vm_area_struct pointer
    struct vm_area_struct *vm_next, *vm_prev;
};

struct mm_struct {
    struct vm_area_struct *mmap;    // head of vm_area_struct list
    uint64_t *pgd;                  // page global directory: pointer to page table.
};
