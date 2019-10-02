#pragma once
#include <stdint.h>

#define THREAD_NUM 6
int thread_number = 0;

extern uint64_t milli_clock;

struct thread_func {
    uint64_t *func;
    int argc;
    char **argv;
};

enum thread_state {
    LIVE,
    DEAD
};

struct thread {
    int thread_id;
    uint64_t *stack;
    uint64_t rsp;
    struct thread_func func_info;
    enum thread_state state;
};

struct thread threads[THREAD_NUM];
int current_thread_index = 0;

// スタックはとりあえず固定配置
uint64_t stack0[0x1000];
uint64_t stack1[0x1000];
uint64_t stack2[0x1000];
uint64_t stack3[0x1000];
uint64_t stack4[0x1000];
uint64_t stack5[0x1000];

extern void save_and_dispatch(uint64_t *current_rsp, uint64_t *next_rsp);
