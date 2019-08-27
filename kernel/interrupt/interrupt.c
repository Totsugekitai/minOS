#include <stdint.h>
#include <interrupt/interrupt.h>
#include <util/util.h>
#include <device/device.h>

struct gate_descriptor make_gate_descriptor(uint64_t offset, uint32_t dpl,
        uint32_t i_or_g, uint32_t ist)
{
    struct gate_descriptor gdesc;

    gdesc.high = (offset & 0xffffffff00000000ull) >> 32;
    gdesc.mid = (offset & 0xffff0000) | (dpl << 13) | (i_or_g << 8) | 0x8e00 | ist;
    gdesc.low = 0x80000 | (offset & 0xffff);

    return gdesc;
}

void init_pic(void)
{
    io_outb(PIC0_ICW1, 0x11);
    io_outb(PIC0_ICW2, 32); // IRQ0-7をINT 0x20-0x27にマップ
    io_outb(PIC0_ICW3, 4);
    io_outb(PIC0_ICW4, 0x01);
    io_outb(PIC0_IMR, 0xff);
    

    io_outb(PIC1_ICW1, 0x11);
    io_outb(PIC1_ICW2, 40); // IRQ8-15をINT 0x28-0x2fにマップ
    io_outb(PIC1_ICW3, 2);
    io_outb(PIC1_ICW4, 0x01);
    io_outb(PIC1_IMR, 0xff);

    io_outb(PIC0_IMR, 0xee); // タイマとCOM1以外はMask
    io_outb(PIC1_IMR, 0xff);
}

