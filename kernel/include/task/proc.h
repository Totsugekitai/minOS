#pragma once
#include <stdint.h>
#include <thread.h>

typedef int pid_t;

struct process {
    pid_t pid;
    struct mm_struct *mm;
    struct thread thread;
};

