#include <stdint.h>

uint64_t pow(uint64_t num, uint64_t pow)
{
    uint64_t i, ans = 1;
    for (i = 0; i < pow; i++) {
        ans *= num;
    }
    return ans;
}
