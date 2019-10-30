#include <stdint.h>
#include <types/boottypes.h>
#include <acpi/acpi.h>
#include <util/util.h>
#include <init/init.h>
#include <interrupt/interrupt.h>
#include <mm/paging.h>
#include <graphics/graphics.h>
#include <debug/debug.h>
#include <device/device.h>
#include <app/app.h>
#include <task/thread.h>

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
void task_a(void);
void task_b(void);
void task_c(void);
void task_input(void);

extern uint64_t stack0[0x1000];
extern uint64_t stack1[0x1000];
extern uint64_t stack2[0x1000];
extern uint64_t stack3[0x1000];
extern uint64_t stack4[0x1000];
extern uint64_t stack5[0x1000];

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

    // タスクスイッチ間隔を設定
    int pe = 300;
    puts_serial("period init: ");
    putnum_serial(pe);
    puts_serial("\n");
    schedule_period_init(pe);
    // threadsを初期化
    threads_init();

    // スレッドを生成
    // コンソールとhltを設定
    struct thread thread0 = thread_gen(stack0, (uint64_t*)task_input, 0, 0);
    // struct thread thread1 = thread_gen(stack1, (uint64_t*)task_a, 0, 0);
    // struct thread thread2 = thread_gen(stack2, (uint64_t*)task_b, 0, 0);
    // struct thread thread3 = thread_gen(stack3, (uint64_t*)task_c, 0, 0);

    // スレッドを走らせる
    thread_run(thread0);
    // thread_run(thread1);
    // thread_run(thread2);
    // thread_run(thread3);
    puts_serial("threads run\n\n");

    puts_serial("next start rsp: ");
    putnum_serial(thread0.rsp);
    puts_serial("\n");
    puts_serial("next start func address: ");
    putnum_serial((uint64_t)(thread0.func_info.func));
    puts_serial("\n");
    puts_serial("next thread rip: ");
    putnum_serial(thread0.rip);
    puts_serial("\n\n");
    puts_serial("first dispatch start\n\n");
    dispatch(thread0.rsp, 0, thread0.rip);

    puts_serial("kernel end.\n");
    while (1) {
        asm("hlt");
    }
}

int taskcharA = 0;
int taskcharB = 0;
int taskcharC = 0;
void task_a(void)
{
    while (1) {
        putchar((taskcharA-8) % 800, 80, white, white, vinfo_global, ' ');
        putchar(taskcharA % 800, 80, white, red, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharA++;
    }
}

void task_b(void)
{
    while (1) {
        putchar((taskcharB-8) % 800, 96, white, white, vinfo_global, ' ');
        putchar(taskcharB % 800, 96, white, blue, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharB += 2;
    }
}

void task_c(void)
{
    while (1) {
        putchar((taskcharC-8) % 800, 112, white, white, vinfo_global, ' ');
        putchar(taskcharC % 800, 112, white, green, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharC += 3;
    }
}

extern uint8_t keycode;
extern uint32_t text_x, text_y;
void task_input(void)
{
    keycode = 0x00; // 初期化
    while (1) {
        keycode = 0x00;
        wait_serial_input();
        // asm volatile(".byte 0xcc");
        if (keycode == 0x0d || keycode == 0x0a)
        {
            puts_serial("new line\n");
            text_y += 16;
            text_x = 0;
        } else {
            puts_serial("put keycode: ");
            putnum_serial(keycode);
            puts_serial("\n\n");
            putchar(text_x, text_y, white, black, vinfo_global, keycode);
            text_x += 8;
        }
    }
}