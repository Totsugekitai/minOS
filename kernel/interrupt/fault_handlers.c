#include <types/boottypes.h>
#include <util/util.h>
#include <core/global_variables.h>
#include <graphics/graphics.h>
#include <device/device.h>
#include <interrupt/int_handler.h>

__attribute__((interrupt))
void de_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#DE ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#DE");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void db_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#DB ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#DB");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void nmi_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("NMI INTERRUPT\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "NMI");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void bp_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#BP ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#BP");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void of_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#OF ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#OF");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void br_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#BR ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#BR");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void ud_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#UD ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#UD");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void nm_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#NM ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#NM");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void df_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
    puts_serial("#DF ERROR CODE = ");
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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#DF");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void ts_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
    puts_serial("#TS ERROR CODE = ");
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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#TS");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void np_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
    puts_serial("#NP ERROR CODE = ");
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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#NP");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void ss_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
    puts_serial("#SS ERROR CODE = ");
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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#SS");
    while (1) asm volatile("hlt");
}


__attribute__((interrupt))
void gp_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
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
    puts_serial("\n\n");

    putstr(600, 500, white, blue, vinfo_global, "#GP");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void pf_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
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
    puts_serial("\n\n");

    putstr(600, 500, white, red, vinfo_global, "#PF");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void mf_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#MF ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, red, vinfo_global, "#MF");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void ac_handler(struct InterruptFrame *frame, uint64_t error_code)
{
    io_cli();
    puts_serial("#AC ERROR CODE = ");
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
    puts_serial("\n\n");

    putstr(600, 500, white, red, vinfo_global, "#AC");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void mc_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#MC ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, red, vinfo_global, "#MC");
    while (1) asm volatile("hlt");
}

__attribute__((interrupt))
void xm_handler(struct InterruptFrame *frame)
{
    io_cli();
    puts_serial("#XM ERROR\n");

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
    puts_serial("\n\n");

    putstr(600, 500, white, red, vinfo_global, "#XM");
    while (1) asm volatile("hlt");
}
