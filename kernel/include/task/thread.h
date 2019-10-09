#pragma once
#include <stdint.h>

#define THREAD_NUM 6 // スレッドの最大数
#define SCHED_THREAD_INDEX 0 // スケジューラのスレッドのインデックス
#define INIT_TIME_SLICE 15 // タイムスライスはとりあえず固定値15

extern uint64_t milli_clock;

struct thread_func {
    void (*func)(int, char**);
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
    struct thread_func func_info;
    enum thread_state state;
};

extern uint64_t stack_init(uint64_t stack_bottom, void (*func)(int, char**));
extern void save_and_dispatch(uint64_t *current_rsp, uint64_t next_rsp);
extern void save_and_dispatch2(uint64_t *current_rsp, uint64_t next_rsp);
extern void dispatch(uint64_t next_rsp);

struct thread thread_gen(uint64_t *stack, void (*func)(int, char**),
    int argc, char **argv);
void thread_run(struct thread thread);
void thread_end(int thread_index);
void threads_init(void);
void schedule_period_init(uint64_t milli_sec);
void thread_scheduler(void);
