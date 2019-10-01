#include <stdint.h>
#include <core/global_variables.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>

void init_interrupt(void)
{
    for (int i = 0; i < 256; i++) {
        IDT[i] = make_gate_descriptor((uint64_t)empty_handler, 0, 0, 0);
    }

    IDT[13] = make_gate_descriptor((uint64_t)gp_handler_deluxe, 0, 0, 0); // #GPハンドラ
    IDT[14] = make_gate_descriptor((uint64_t)page_fault, 0, 0, 0); // #PFハンドラ
    IDT[32] = make_gate_descriptor((uint64_t)timer_handler, 0, 0, 0); // timerハンドラ
    IDT[33] = make_gate_descriptor((uint64_t)keyboard_handler, 0, 0, 0); // keyboardハンドラ
    IDT[36] = make_gate_descriptor((uint64_t)com1_handler, 0, 0, 0); // COM1ハンドラ
    IDT[44] = make_gate_descriptor((uint64_t)mouse_handler, 0, 0, 0); // マウスハンドラ

    load_idt((uint64_t)IDT, sizeof(struct gate_descriptor) * 256 - 1);
}

