#pragma once
#include <stdint.h>

struct malloc_chunk {
    int size;
    struct malloc_chunk *next;
    uint64_t *data;
};

uint64_t *minmalloc(int size);