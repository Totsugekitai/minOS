#include <stdint.h>
#include <util/util.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <mm/memory.h>

#define HEAP_SIZE 0x400000
static uint64_t heap[HEAP_SIZE];

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
    puts_serial("start minmalloc.\n");

    // calculate block size
    uint64_t number_of_blocks =
        ((size + sizeof(struct malloc_header) - 1) / sizeof(struct malloc_header)) + 1;

    put_str_num_serial("number of blocks: ", number_of_blocks);

    struct malloc_header *p, *prev_p = next_search_position;

    // allocate
    //first malloc
    if (prev_p == 0) {
        puts_serial("first malloc!\n");

        next_search_position = prev_p = &base;
        base.block_size = 0;
        base.next = (struct malloc_header *)heap;
        base.next->block_size =
            (HEAP_SIZE / sizeof(struct malloc_header));
        base.next->next = &base;

        puts_serial("init base\n");
        put_str_num_serial("base: ", (uint64_t)&base);
        put_str_num_serial("base.block_size: ", base.block_size);
        put_str_num_serial("base.next: ", (uint64_t)base.next);
        puts_serial("\n");

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
            put_str_num_serial("prev_p: ", (uint64_t)prev_p);
            put_str_num_serial("prev_p->block_size: ", prev_p->block_size);
            put_str_num_serial("prev_p->next: ", (uint64_t)prev_p->next);
            put_str_num_serial("p: ", (uint64_t)p);
            put_str_num_serial("p->block_size: ", p->block_size);
            put_str_num_serial("p->next: ", (uint64_t)p->next);
            puts_serial("\n");
            return ((void *)(p + 1));
        }
    }
    // normal malloc
    else {
        puts_serial("malloc\n");
        for (p = prev_p->next;; prev_p = p, p = p->next) {
            if (p->block_size >= number_of_blocks) {
                if (p->block_size == number_of_blocks) {
                    prev_p->next = p->next;
                } else {
                    next_search_position = prev_p;
                    p->block_size -= number_of_blocks;
                    p += p->block_size;
                    p->block_size = number_of_blocks;
                }
                put_str_num_serial("prev_p: ", (uint64_t)prev_p);
                put_str_num_serial("prev_p->block_size: ", prev_p->block_size);
                put_str_num_serial("prev_p->next: ", (uint64_t)prev_p->next);
                put_str_num_serial("p: ", (uint64_t)prev_p);
                put_str_num_serial("p->block_size: ", p->block_size);
                put_str_num_serial("p->next: ", (uint64_t)p->next);
                puts_serial("\n");
                return ((void *)(p + 1));
            }
            if (p == next_search_position) {
                return 0;
            }
        }
    }
    return 0;
}
