#pragma once
#include <stdint.h>

#define MAX_TASKS   16
#define MAX_ARGS 8
#define ARG_LENGTH 16
#define OUTPUT_LENGTH 64
#define READLINE_BUF_LENGTH 64
#define READLINE_BUF_LENGTH 64

extern struct task_queue task_q;
extern volatile int readline_flag;
extern volatile char readline_buf[READLINE_BUF_LENGTH];

struct task {
    int is_checked;
    uint64_t exec_time;
    char *command_line;
};

struct task_queue {
    struct task tasks[MAX_TASKS];
};

void console(int argc, char **argv);

