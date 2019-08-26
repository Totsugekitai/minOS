#include <stdint.h>
#include <app/app.h>

extern uint64_t milli_clock;
extern struct task_queue task_q;

void delay(uint64_t time, char *command_line)
{
    time = time + milli_clock;
    struct task task = {0, time, command_line};
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_q.tasks[i].command_line == 0) {
            task_q.tasks[i] = task;
            break;
        }
    }
}

