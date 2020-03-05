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

/**
 * タイマ割り込みハンドラ
 * クロック値を進めて、スケジューラを呼び出す
*/
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
    if (milli_clock > previous_interrupt + timer_period && milli_clock > 900) {
        previous_interrupt = milli_clock;
        // put_str_num_serial("timer_handler old rip: ", frame->rip);
        // puts_serial("\n");
        thread_scheduler();
    }
}

/**
 * PS/2キーボードの割り込みハンドラ
*/
__attribute__((interrupt))
void keyboard_handler(struct InterruptFrame *frame)
{
    putnum_serial(io_inb(0x60));
    puts_serial("keyboard interrupt\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

/**
 * シリアルポートCOM1の割り込みハンドラ
*/
__attribute__((interrupt))
void com1_handler(struct InterruptFrame *frame)
{
    io_cli();
    keycode = io_inb(PORT);
    puts_serial("Interrupt COM1\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);

    io_sti();
}

/**
 * PS/2マウス割り込みハンドラ
 * 現在は未使用
*/
__attribute__((interrupt))
void mouse_handler(struct InterruptFrame *frame)
{
    puts_serial("mouse interrupt\n");
    io_outb(PIC0_OCW2, PIC_EOI);
    io_outb(PIC1_OCW2, PIC_EOI);
}

/**
 * 空のハンドラ
 * 未使用の割り込みベクタに登録しておく
*/
__attribute__((interrupt))
void empty_handler(struct InterruptFrame *frame)
{
    puts_serial("empty handler\n");
    while (1) {
        asm("hlt");
    }
}
