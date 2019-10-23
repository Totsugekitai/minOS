#include <types/boottypes.h>
#include <core/global_variables.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <interrupt/int_handler.h>

__attribute__((interrupt))
void gp_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    puts_serial("#GP ERROR CODE = ");
    putnum_serial(error_code);
    puts_serial("\n");

    puts_serial("RIP: ");
    putnum_serial(frame->rip);
    puts_serial("\n");
    puts_serial("CS: ");
    putnum_serial(frame->cs);
    puts_serial("\n");
    puts_serial("RFLAGS: ");
    putnum_serial(frame->rflags);
    puts_serial("\n");
    puts_serial("RSP: ");
    putnum_serial(frame->rsp);
    puts_serial("\n");
    puts_serial("SS: ");
    putnum_serial(frame->ss);
    puts_serial("\n");

    putstr(600, 70, white, blue, vinfo_global, "#GP");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void pf_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    puts_serial("#PF ERROR CODE = ");
    putnum_serial(error_code);
    puts_serial("\n");

    puts_serial("RIP: ");
    putnum_serial(frame->rip);
    puts_serial("\n");
    puts_serial("CS: ");
    putnum_serial(frame->cs);
    puts_serial("\n");
    puts_serial("RFLAGS: ");
    putnum_serial(frame->rflags);
    puts_serial("\n");
    puts_serial("RSP: ");
    putnum_serial(frame->rsp);
    puts_serial("\n");
    puts_serial("SS: ");
    putnum_serial(frame->ss);
    puts_serial("\n");

    putstr(600, 70, white, red, vinfo_global, "#PF");
    while (1) {
        asm("hlt");
    }
}
