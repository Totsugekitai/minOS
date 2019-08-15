#include <stdint.h>
#include <types/boottypes.h>
#include <acpi/acpi.h>
#include <util/util.h>
#include <init/initfunc.h>
#include <mm/segmentation.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>
#include <mm/paging.h>
#include <graphics/graphics.h>
#include <debug/debug.h>
#include <device/device.h>
#include <app/app.h>

struct pix_format black = {0x00, 0x00, 0x00, 0x00};
struct pix_format white = {0xFF, 0xFF, 0xFF, 0x00};
struct pix_format red = {0x00, 0x00, 0xFF, 0x00};
struct pix_format green = {0x00, 0xFF, 0x00, 0x00};
struct pix_format blue = {0xFF, 0x00, 0x00, 0x00};

struct video_info *vinfo_global = (struct video_info *)0; // とりあえず0入れとく
struct RSDP *rsdp;

void main_routine(struct video_info *vinfo);

void start_kernel(struct bootinfo *binfo)
{
    // シリアル通信初期化
    init_serial();
    
    puts_serial("\nstart_kernel\n"); // debug用
    // タイマ割り込み有効化
    //init_local_APIC();

    
    rsdp = binfo->rsdp;

    puts_serial("RSDP: ");
    putnum_serial((uint64_t)rsdp);
    puts_serial("\n");

    vinfo_global = &(binfo->vinfo);
    puts_serial("vinfo_global dainyuu\n"); // debug用
    
    /* BSSセクションの初期化 */
    init_bss();
    
    puts_serial("bss init\n"); // debug用
    
    /* GDTの初期化 */
    // GDTの先頭アドレスは0x80
    uint64_t *GDT = (uint64_t *)0x80;
    // 空, KERNEL_CS, USER_CSの3つを用意
    GDT[0] = make_segment_descriptor(0, 0, 1);
    GDT[1] = make_segment_descriptor(5, 0, 0);
    GDT[2] = make_segment_descriptor(5, 3, 0);
    // lgdtでGDTをセットする
    
    puts_serial("GDTR written\n"); // debug用
    
    load_gdt(GDT, 0x17);
    
    puts_serial("loaded gdt\n"); // debug用
    
    // セグメントレジスタの設定をやってmain_routineへとぶ
    set_segment_register(0x8, vinfo_global);
}

/* GDTの設定が終わった後のルーチン */
void main_routine(struct video_info *vinfo)
{
    puts_serial("reach main_routine\n"); // debug用
    /* ページングの初期化 */
    uint64_t *PML4 = (uint64_t *)0x1000;
    uint64_t *PDP = (uint64_t *)0x2000;
    uint64_t *PD = (uint64_t *)0x10000;
    create_pgtable(PML4, PDP, PD);
    load_pgtable(PML4);

    puts_serial("set paging\n"); // debug用

    /* IDTの初期化 */
    // IDTの先頭アドレスは0x20000
    struct gate_descriptor *IDT = (struct gate_descriptor *)0x20000;
    struct gate_descriptor zero_gate = make_gate_descriptor(0, 0, 0, 0);
    for (int i = 0; i < 13; i++) {
        IDT[i] = zero_gate;
    }
    IDT[13] = make_gate_descriptor((uint64_t)gp_handler_deluxe, 0, 0, 0);
    IDT[14] = make_gate_descriptor((uint64_t)page_fault, 0, 0, 0);
    for (int i = 15; i < 32; i++) {
        IDT[i] = zero_gate;
    }
    IDT[32] = make_gate_descriptor((uint64_t)timer_handler, 0, 0, 0);
    for (int i = 33; i < 256; i++) {
        IDT[i] = make_gate_descriptor((uint64_t)timer_handler, 0, 0, 0);
    }
    load_idt((uint64_t)IDT, sizeof(struct gate_descriptor) * 256);

    puts_serial("load IDT\n"); // debug用

    // フレームバッファの初期化
    uint32_t i, j;
    uint32_t x_axis = vinfo_global->x_axis;
    uint32_t y_axis = vinfo_global->y_axis;
    uint32_t ppsl = vinfo_global->ppsl;
    struct pix_format *fb = vinfo_global->fb;
    for (i = 0; i < y_axis; i++) {
        for (j = 0; j < x_axis; j++) {
            fb[i * ppsl + j] = white;
        }
    }
    
    puts_serial("init framebuffer\n"); // debug用

    // タイマ割り込み有効化
    init_local_APIC();

    /* いろんなレジスタとかメモリとかの表示 */
    // EFER
    putstr(600, 10, black, white, vinfo, "CR3: ");
    putnum(650, 10, black, white, vinfo, get_cr3());
    // EFER
    putstr(600, 30, black, white, vinfo, "CR4: ");
    putnum(650, 30, black, white, vinfo, get_cr4());
    // EFER
    putstr(600, 50, black, white, vinfo, "EFER: ");
    putnum(650, 50, black, white, vinfo, get_efer());
    // 自由欄
    //for (int i = 0; i < 4; i++) {
    //    putchar(600 + i * 8, 70, black, white, vinfo, fadt->header.signature[i]);
    //}

    /* 名前 */
    putstr(515, 560, black, white, vinfo,
           "minOS - A Minimal Operating System.");
    putstr(500, 580, black, white, vinfo,
           "Developer : Totsugekitai(@totsugeki8)");

    while (1) {
        console();
    }
}
