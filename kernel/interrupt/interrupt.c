#include <stdint.h>
#include <interrupt/interrupt.h>

struct gate_descriptor make_gate_descriptor(uint64_t offset, uint32_t dpl,
        uint32_t i_or_g, uint32_t ist)
{
    struct gate_descriptor gdesc;

    gdesc.high = (offset & 0xffffffff00000000ull) >> 32;
    gdesc.mid = (offset & 0xffff0000) | (dpl << 13) | (i_or_g << 8) | 0xd00 | ist;
    gdesc.low = 0x10000 | (offset & 0xffff);

    return gdesc;
}
