#pragma once
#include <stdint.h>

struct malloc_header {
    uint64_t block_size;
    struct malloc_header *next;
};

void *minmalloc(uint64_t size);