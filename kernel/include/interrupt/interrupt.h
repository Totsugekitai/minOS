#pragma once
#include <stdint.h>
struct gate_descriptor {
    uint32_t low;
    uint32_t mid;
    uint64_t high;
};

struct gate_descriptor make_gate_descriptor(uint64_t offset, uint32_t dpl,
        uint32_t i_or_g, uint32_t ist);
