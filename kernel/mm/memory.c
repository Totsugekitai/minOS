#include <stdint.h>
#include <util/util.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <mm/memory.h>

// These variables are about heap region.
// When use these for address calculation, attach & to its head.
extern uint64_t __heap_start, __heap_end;
extern uint64_t _heap_size;

// This global variable is base pointer of malloc.
// This has only chunk header, don't have data.
struct malloc_chunk base_chunk = {
    0,
    0,
    0
};

uint64_t *minmalloc(int size)
{
    // TODO
}