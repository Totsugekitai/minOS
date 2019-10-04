#pragma once
#include <stdint.h>

#define THREAD_NUM 6 // スレッドの最大数
#define SCHED_THREAD_INDEX 0 // スケジューラのスレッドのインデックス
#define INIT_TIME_SLICE 15 // 初期化時のタイムスライス

extern uint64_t milli_clock;

struct thread_func {
    uint64_t *func;
    int argc;
    char **argv;
};

enum thread_state {
    RUNNABLE,
    SLEEP,
    DEAD
};

struct thread {
    uint64_t *stack;
    uint64_t rsp;
    uint64_t timeslice;
    struct thread_func func_info;
    enum thread_state state;
};

/** threadsについて
 * スレッドの配列がそのままランキュー
 * 0番目は必ずスレッドスケジューラのスレッドにする
 */
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
int search_next_thread(void);