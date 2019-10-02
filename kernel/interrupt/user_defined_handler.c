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
    
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

__attribute__((interrupt))
void mouse_handler(struct InterruptFrame *frame)
{
    puts_serial("mouse interrupt\n");
    io_outb(PIC0_OCW2, 0x20);
    io_outb(PIC1_OCW2, 0x20);
}

__attribute__((interrupt))
void empty_handler(struct InterruptFrame *frame)
{
    puts_serial("empty handler\n");
}

