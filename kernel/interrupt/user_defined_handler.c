#include <stdint.h>
#include <types/boottypes.h>
#include <device/device.h>
#include <graphics/graphics.h>
#include <interrupt/int_handler.h>
#include <app/app.h>

#define READLINE_BUF_LENGTH     64

uint64_t milli_clock = 0;
uint64_t *end_of_interrupt = (uint64_t *)0xfee000b0;
extern struct pix_format black;
extern struct pix_format white;
extern struct video_info *vinfo_global;
extern struct task_queue task_q;
extern int readline_flag;
extern char readline_buf[READLINE_BUF_LENGTH];

__attribute__((interrupt))
void timer_handler(struct InterruptFrame *frame)
{
    milli_clock += 1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_q.tasks[i].is_checked == 0 &&
            task_q.tasks[i].exec_time != 0 &&
            task_q.tasks[i].exec_time < milli_clock) {
            for (int i = 0; i < READLINE_BUF_LENGTH; i++) {
                if (task_q.tasks[i].command_line[i] == 0x00) {
                    break;
                }
                readline_buf[i] = task_q.tasks[i].command_line[i];
            }
            task_q.tasks[i].is_checked = 1;
            readline_flag = 1;
            puts_serial("readline_flag changes 1\n");
            break;
        }
    }
    *end_of_interrupt = 0;
}

