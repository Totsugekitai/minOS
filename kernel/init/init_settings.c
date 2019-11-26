#include <stdint.h>
#include <mm/segmentation.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>
#include <util/util.h>
#include <core/global_variables.h>
#include <mm/paging.h>

// These are from linker script
// Normally, variables from linker script is decleared as not pointer type
extern uint64_t __bss_start, __bss_end;

// init bss section
void init_bss(void)
{
    int bss_size = (int)(&__bss_end - &__bss_start);
    for (int k = 0; k < bss_size; k++) {
        (&__bss_start)[k] = 0x00;
    }
}

// init global variables
void init_global_variables(struct bootinfo *binfo)
{
    rsdp = binfo->rsdp;
    vinfo_global = &binfo->vinfo;
    start_mmap = binfo->memory_descriptor;
    mmapsize = binfo->mmapsize;
    memdescsize = binfo->memdescsize;
}

// init GDT
void init_gdt(void)
{
    // 空, KERNEL_CS, KERNEL_DSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(1, 0, 0);
    load_gdt(GDT, 0x18);
    // セグメントレジスタの設定をやってmain_routineへとぶ
    set_segment_register(0x8);
}

// ページングの初期化
void init_paging(void)
{
    create_pgtable(PML4, PDP, PD);
    load_pgtable(PML4);
}

// グラフィックの初期化
void init_graphics(void)
{
    uint32_t x_axis = vinfo_global->x_axis;
    uint32_t y_axis = vinfo_global->y_axis;
    uint32_t ppsl = vinfo_global->ppsl;
    struct pix_format *fb = vinfo_global->fb;
    for (uint32_t i = 0; i < y_axis; i++)
    {
        for (uint32_t j = 0; j < x_axis; j++)
        {
            fb[i * ppsl + j] = white;
        }
    }
}

// 割り込みハンドラの初期化
void init_interrupt(void)
{
    for (int i = 0; i < 256; i++)
    {
        IDT[i] = make_gate_descriptor((uint64_t)empty_handler, 0, 0, 0);
    }

    IDT[0] = make_gate_descriptor((uint64_t)de_handler, 0, 0, 0);        // #DEハンドラ
    IDT[1] = make_gate_descriptor((uint64_t)db_handler, 0, 0, 0);        // #DBハンドラ
    IDT[2] = make_gate_descriptor((uint64_t)nmi_handler, 0, 0, 0);       // NMIハンドラ
    IDT[3] = make_gate_descriptor((uint64_t)bp_handler, 0, 0, 0);        // #BPハンドラ
    IDT[4] = make_gate_descriptor((uint64_t)of_handler, 0, 0, 0);        // #OFハンドラ
    IDT[5] = make_gate_descriptor((uint64_t)br_handler, 0, 0, 0);        // #BRハンドラ
    IDT[6] = make_gate_descriptor((uint64_t)ud_handler, 0, 0, 0);        // #UDハンドラ
    IDT[7] = make_gate_descriptor((uint64_t)nm_handler, 0, 0, 0);        // #NMハンドラ
    IDT[8] = make_gate_descriptor((uint64_t)df_handler, 0, 0, 0);        // #DFハンドラ
    IDT[10] = make_gate_descriptor((uint64_t)ts_handler, 0, 0, 0);       // #TSハンドラ
    IDT[11] = make_gate_descriptor((uint64_t)np_handler, 0, 0, 0);       // #NPハンドラ
    IDT[12] = make_gate_descriptor((uint64_t)ss_handler, 0, 0, 0);       // #SSハンドラ
    IDT[13] = make_gate_descriptor((uint64_t)gp_handler, 0, 0, 0);       // #GPハンドラ
    IDT[14] = make_gate_descriptor((uint64_t)pf_handler, 0, 0, 0);       // #PFハンドラ
    IDT[16] = make_gate_descriptor((uint64_t)mf_handler, 0, 0, 0);       // #MFハンドラ
    IDT[17] = make_gate_descriptor((uint64_t)ac_handler, 0, 0, 0);       // #ACハンドラ
    IDT[18] = make_gate_descriptor((uint64_t)mc_handler, 0, 0, 0);       // #MCハンドラ
    IDT[19] = make_gate_descriptor((uint64_t)xm_handler, 0, 0, 0);       // #XMハンドラ
    IDT[32] = make_gate_descriptor((uint64_t)timer_handler, 0, 0, 0);    // timerハンドラ
    IDT[33] = make_gate_descriptor((uint64_t)keyboard_handler, 0, 0, 0); // keyboardハンドラ
    IDT[36] = make_gate_descriptor((uint64_t)com1_handler, 0, 0, 0);     // COM1ハンドラ
    IDT[44] = make_gate_descriptor((uint64_t)mouse_handler, 0, 0, 0);    // マウスハンドラ

    load_idt((uint64_t)IDT, sizeof(struct gate_descriptor) * 256 - 1);
}

// MSI割り込みの初期化
// uint8_t *get_pci_capability_pointer(void)
// {
//     // PCI Configuration Registerを読み出し
//     uint32_t config_address_register = io_in32(0xcf8);
//     // Enable bitを1にして書き込み
//     config_address_register |= 80000000;
//     io_out32(0xcf8, config_address_register);
// 
//     uint8_t *capability_pointer;
//     return capability_pointer;
// }
// 
// void init_msi(void)
// {
//     uint8_t *capability_pointer = get_pci_capability_pointer();
// }

