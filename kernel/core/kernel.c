#include <stdint.h>
#include <types/boottypes.h>
#include <acpi/acpi.h>
#include <util/util.h>
#include <init/init.h>
#include <interrupt/interrupt.h>
#include <interrupt/int_handler.h>
#include <mm/segmentation.h>
#include <mm/paging.h>
#include <mm/memory.h>
#include <graphics/graphics.h>
#include <debug/debug.h>
#include <device/device.h>
#include <app/app.h>

// bootinfoからとれる情報
struct video_info *vinfo_global;
struct RSDP *rsdp;
struct memory_descriptor *start_mmap;
uint64_t mmapsize;
uint64_t memdescsize;
// GDTは0x80に固定配置
uint64_t *GDT = (uint64_t *)0x80;
// 初期ページテーブルは固定配置
uint64_t *PML4 = (uint64_t *)0x1000;
uint64_t *PDP = (uint64_t *)0x2000;
uint64_t *PD = (uint64_t *)0x3000;
// IDTは固定配置
struct gate_descriptor *IDT = (struct gate_descriptor *)0x13000;

void main_routine(void);

void start_kernel(struct bootinfo *binfo)
{
    init_bss();
    init_serial();
    init_global_variables(binfo);
    init_gdt(); // init_gdtでmain_routineへジャンプする
}

/* GDTの設定が終わった後のルーチン */
void main_routine(void)
{
    init_paging();
    init_interrupt();
    init_graphics();
    //init_local_APIC();
    init_pic();

    /* いろんなレジスタとかメモリとかの表示 */
    // EFER
    putstr(600, 10, black, white, vinfo_global, "CR3: ");
    putnum(650, 10, black, white, vinfo_global, get_cr3());
    // EFER
    putstr(600, 30, black, white, vinfo_global, "CR4: ");
    putnum(650, 30, black, white, vinfo_global, get_cr4());
    // EFER
    putstr(600, 50, black, white, vinfo_global, "EFER: ");
    putnum(650, 50, black, white, vinfo_global, get_efer());
    /* 名前 */
    putstr(515, 560, black, white, vinfo_global,
           "minOS - A Minimal Operating System.");
    putstr(500, 580, black, white, vinfo_global,
           "Developer : Totsugekitai(@totsugeki8)");
    
    putstr(0, 16, black, white, vinfo_global, "mmapsize: ");
    putnum(100, 16, black, white, vinfo_global, mmapsize);

    putstr(0, 32, black, white, vinfo_global, "memdescsize: ");
    putnum(100, 32 ,black, white, vinfo_global, memdescsize);

    // int i = 0, j = 48;
    // uint64_t numpages;
    // uint64_t phystart;
    // uint64_t virtstart;
    // do {
    //     numpages = start_mmap[i].number_of_pages;
    //     phystart = start_mmap[i].physical_start;
    //     virtstart = start_mmap[i].virtual_start;
    //     putnum(0, j, black, white, vinfo_global, numpages);
    //     putstr(170, j, black, white, vinfo_global, ":");
    //     putnum(180, j, black, white, vinfo_global, phystart);
    //     putstr(350, j, black, white, vinfo_global, ":");
    //     putnum(360, j, black, white, vinfo_global, virtstart);
    //     i++;
    //     j += 16;
    // } while (i != 10);

    // 1GBまで検査
    //volatile uint64_t memsize = calc_mem_size(0, 0x10000);
    //putstr(0, 64, black, white, vinfo_global, "memsize:");
    //putnum(70, 64, black, white, vinfo_global, memsize);

    // コンソール
    puts_serial("console start\n");
    console(0, 0);
}

