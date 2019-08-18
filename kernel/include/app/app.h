#pragma once
#include <stdint.h>

#define MAX_TASKS   16

struct task {
    int is_checked;
    uint64_t exec_time;
    char *command_line;
};

struct task_queue {
    struct task tasks[MAX_TASKS];
};

void console(void);

