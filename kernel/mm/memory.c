#include <stdint.h>

#define CR0_CACHE_DISABLE   0x60000000

extern uint32_t load_cr0(void);
extern void store_cr0(uint32_t);

uint64_t calc_mem_size_sub(uint64_t start, uint64_t end);

uint64_t calc_mem_size(uint64_t start, uint64_t end)
{
    uint32_t cr0;

    // まずはキャッシュを禁止する
    cr0 = load_cr0();
    cr0 |= CR0_CACHE_DISABLE;
    store_cr0(cr0);

    // サイズを調べる
    volatile uint64_t size = calc_mem_size_sub(start, end);

    // キャッシュをもとに戻す
    cr0 = load_cr0();
    cr0 &= ~CR0_CACHE_DISABLE;
    store_cr0(cr0);

    return size;
}

uint64_t calc_mem_size_sub(uint64_t start, uint64_t end)
{
    uint64_t i, *p, old;
    uint64_t pat0 = 0xaa55aa55aa55aa55ull, pat1 = 0x55aa55aa55aa55aaull;

    for (i = start; i <= end; i += 0x1000) {
        p = (uint64_t *)(i + 0xffc);
        old = *p;
        *p = pat0;
        *p ^= 0xffffffffffffffffull;
        if (*p != pat1) {
            *p = old;
            break;
        }
        *p ^= 0xffffffffffffffffull;
        if (*p != pat0) {
            *p = old;
            break;
        }
        *p = old;
    }

    return i;
}

