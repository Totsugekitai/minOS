#include <stdint.h>
#include <core/global_variables.h>
#include <types/boottypes.h>
#include <device/device.h>
#include <graphics/graphics.h>
#include <interrupt/int_handler.h>
#include <interrupt/interrupt.h>
#include <app/app.h>
#include <util/util.h>

#define PORT    0x3f8

uint64_t milli_clock = 0;
uint64_t *end_of_interrupt = (uint64_t *)0xfee000b0;

extern uint8_t keycode;

__attribute__((interrupt))
void timer_handler(struct InterruptFrame *frame)
{
    milli_clock += 1;
    puts_serial("timer\n");
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
    //for (int i = 0; i < MAX_TASKS; i++) {
    //    if (task_q.tasks[i].is_checked == 0 &&
    //        task_q.tasks[i].exec_time != 0 &&
    //        task_q.tasks[i].exec_time < milli_clock) {
    //        for (int i = 0; i < READLINE_BUF_LENGTH; i++) {
    //            if (task_q.tasks[i].command_line[i] == 0x00) {
    //                break;
    //            }
    //            readline_buf[i] = task_q.tasks[i].command_line[i];
    //        }
    //        task_q.tasks[i].is_checked = 1;
    //        readline_flag = 1;
    //        puts_serial("readline_flag changes 1\n");
    //        break;
    //    }
    //}
    //*end_of_interrupt = 0;
}

void timer_handler_dash(void)
{
    milli_clock += 1;
    //puts_serial("timer\n");
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

__attribute__((interrupt))
void keyboard_handler(struct InterruptFrame *frame)
{
    putnum_serial(io_inb(0x60));
    puts_serial(" keyboard interrupt\n");
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

void keyboard_handler_dash(void)
{
    puts_serial("keyboard interrupt\n");
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

__attribute__((interrupt))
void com1_handler(struct InterruptFrame *frame)
{
    keycode = io_inb(PORT);
    putnum_serial(keycode);
    puts_serial(" COM1 interrupt.\n");
    
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

void com1_handler_dash(void)
{
    puts_serial("COM1 interrupt.\n");
    keycode = io_inb(PORT);
    io_outb(PIC0_OCW2, 0x61); // IRQ-04受付完了をPICに通知
    
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

