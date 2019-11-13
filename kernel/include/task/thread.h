#pragma once
#include <stdint.h>

#define THREAD_NUM 4 // スレッドの最大数
#define SCHED_THREAD_INDEX 0 // スケジューラのスレッドのインデックス

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
    uint64_t rip;
    struct thread_func func_info;
    enum thread_state state;
};

extern uint64_t init_stack(uint64_t stack_bottom, uint64_t rip);
extern void dispatch(uint64_t next_rsp, uint64_t *current_rsp, uint64_t next_rip);
extern uint64_t init_stack2(uint64_t stack_bottom, uint64_t rip);
extern void switch_context(uint64_t *current_rsp, uint64_t next_rsp);

struct thread thread_gen(uint64_t *stack, uint64_t *func,
    int argc, char **argv);
void thread_run(struct thread thread);
void thread_end(int thread_index);
void threads_init(void);
void schedule_period_init(uint64_t milli_sec);
void thread_scheduler(uint64_t next_rip);
