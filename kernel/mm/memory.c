#include <stdint.h>
#include <util/util.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <mm/memory.h>

// These variables are about heap region.
// When use these for address calculation, attach & to its head.
extern uint64_t __heap_start, __heap_end;

// This global variable is base pointer of malloc.
// This has only chunk header, don't have data.
static struct malloc_header base = {
    0,
    0
};

static struct malloc_header *next_search_position = 0;

// This is memory allocator based on K&R malloc.
// The argument "size", which unit is "byte", don't include header size.
void *minmalloc(uint64_t size)
{
    // calculate block size
    uint64_t number_of_blocks =
        ((size + sizeof(struct malloc_header) - 1) / sizeof(struct malloc_header)) + 1;

    struct malloc_header *p, *prev_p = next_search_position;

    // allocate
    //first malloc
    if (prev_p == 0) {
        next_search_position = prev_p = &base;
        base.block_size = 0;
        base.next = &__heap_start;
        base.next->block_size =
            ((uint64_t)(&__heap_end - &__heap_start) / sizeof(struct malloc_header));
        base.next->next = &base;

        p = prev_p->next;
        // check malloc size
        if (p->block_size >= number_of_blocks) {    // big enough
            if (p->block_size == number_of_blocks) {    // exactly
                prev_p->next = p->next;
            } else {    // allocate tail end
                next_search_position = p;
                p->block_size -= number_of_blocks;
                p += p->block_size;
                p->block_size = number_of_blocks;
            }
            return ((void *)(p + 1));
        }
    }
    // normal malloc
    else {
        for (p = prev_p->next;; prev_p = p, p = p->next) {
            if (p->block_size >= number_of_blocks) {
                if (p->block_size == number_of_blocks) {
                    prev_p->next = p->next;
                } else {
                    p->block_size -= number_of_blocks;
                    p += p->block_size;
                    p->block_size = number_of_blocks;
                }
                next_search_position = prev_p;
                return ((void *)(p + 1));
            }
            if (p == next_search_position) {
                return 0;
            }
        }
    }
}