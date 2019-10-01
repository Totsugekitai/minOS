#pragma once
#include <stdint.h>

struct task_state {
    uint64_t rsp;
    uint64_t rip;
    uint64_t rax, rcx, rdx, rbx, rdi, rsi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t rflags;
};

