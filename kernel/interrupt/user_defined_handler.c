#include <stdint.h>
#include <core/global_variables.h>
#include <types/boottypes.h>
#include <device/device.h>
#include <graphics/graphics.h>
#include <interrupt/int_handler.h>
#include <interrupt/interrupt.h>
#include <app/app.h>
#include <util/util.h>
#include <task/thread.h>

#define PORT    0x3f8
#define PIC_EOI 0x20

uint64_t milli_clock = 0;
uint64_t *end_of_interrupt = (uint64_t *)0xfee000b0;

extern uint8_t keycode;

extern uint64_t previous_interrupt;
extern uint64_t timer_period;

void my_timer_handler(void)
{
    // クロック値を進める
    milli_clock++;
    // EOIをPICに送る
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);

    /** 周期が来たらスケジューラを呼び出す
     * 各種パラメータはint_handler.hで設定
     */
    // if (milli_clock > previous_interrupt + timer_period && milli_clock > 100) {
    //     previous_interrupt = milli_clock;
    //     thread_scheduler();
    // }
}

__attribute__((interrupt))
void timer_handler(struct InterruptFrame *frame)
{
    // クロック値を進める
    milli_clock++;
    // EOIをPICに送る
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);

    /** 周期が来たらスケジューラを呼び出す
     * 各種パラメータはint_handler.hで設定
     */
    if (milli_clock > previous_interrupt + timer_period && milli_clock > 100) {
        previous_interrupt = milli_clock;
        thread_scheduler(frame->rip);
    }
}

__attribute__((interrupt))
void keyboard_handler(struct InterruptFrame *frame)
{
    putnum_serial(io_inb(0x60));
    puts_serial(" keyboard interrupt\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

void keyboard_handler_dash(void)
{
    puts_serial("keyboard interrupt\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

__attribute__((interrupt))
void com1_handler(struct InterruptFrame *frame)
{
    keycode = io_inb(PORT);
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);

    putnum_serial(keycode);
    puts_serial(" COM1 interrupt.\n");
}

void com1_handler_dash(void)
{
    puts_serial("COM1 interrupt.\n");
    keycode = io_inb(PORT);
    
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

__attribute__((interrupt))
void mouse_handler(struct InterruptFrame *frame)
{
    puts_serial("mouse interrupt\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

__attribute__((interrupt))
void empty_handler(struct InterruptFrame *frame)
{
    puts_serial("empty handler\n");
}

