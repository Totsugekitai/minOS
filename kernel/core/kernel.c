#include <stdint.h>
#include <types/boottypes.h>
#include <acpi/acpi.h>
#include <util/util.h>
#include <init/init.h>
#include <interrupt/interrupt.h>
#include <mm/paging.h>
#include <mm/memory.h>
#include <graphics/graphics.h>
#include <debug/debug.h>
#include <device/device.h>
#include <device/sata.h>
#include <app/app.h>
#include <task/thread.h>

// These are information that we get at bootinfo
struct video_info *vinfo_global;
struct RSDP *rsdp;
struct memory_descriptor *start_mmap;
uint64_t mmapsize;
uint64_t memdescsize;
// GDT is located at 0x80
uint64_t *GDT = (uint64_t *)0x80;
// Init page table is located at fixed position
uint64_t *PML4 = (uint64_t *)0x1000;
uint64_t *PDP = (uint64_t *)0x2000;
uint64_t *PD = (uint64_t *)0x3000;
// IDT is located at fixed position
struct gate_descriptor *IDT = (struct gate_descriptor *)0x13000;

void main_routine(void);
void task_a(int _argc, char **_argv);
void task_b(int _argc, char **_argv);
void task_c(int _argc, char **_argv);

void start_kernel(struct bootinfo *binfo)
{
    init_bss();
    init_serial();
    init_global_variables(binfo);
    init_gdt(); // jump main_routine!!!
}

/* This is a routine after finishing setting GDT */
void main_routine(void)
{
    init_paging();
    init_interrupt();
    init_graphics();
    //init_local_APIC();
    init_pic();

    /* print various memory dump and registers */
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
    putstr(480, 580, black, white, vinfo_global,
           "Developer : Totsugekitai(@totsugeki_tai)");

    check_all_buses();
    check_ahci();

    while (1) {
        asm volatile("hlt");
    }

    // set task switch interval
    int pe = 1;
    put_str_num_serial("period init: ", pe);
    schedule_period_init(pe);
    // threadsを初期化
    threads_init();

    // スレッドを生成
    struct thread thread0 = thread_gen(console, 0, 0);
    struct thread thread1 = thread_gen(task_a, 0, 0);
    struct thread thread2 = thread_gen(task_b, 0, 0);
    struct thread thread3 = thread_gen(task_c, 0, 0);

    // initialize stack
    thread_stack_init(&thread0);
    thread_stack_init(&thread1);
    thread_stack_init(&thread2);
    thread_stack_init(&thread3);

    // run thread
    thread_run(&thread0);
    thread_run(&thread1);
    thread_run(&thread2);
    thread_run(&thread3);
    puts_serial("threads run\n\n");

    put_str_num_serial("next start rsp: ", thread0.rsp);
    put_str_num_serial("next start func address: ", (uint64_t)thread0.func_info.func);
    put_str_num_serial("next thread rip: ", thread0.rip);
    puts_serial("first dispatch start\n\n");

    switch_context(0, thread0.rsp);

    puts_serial("kernel end.\n");
    while (1) {
        asm("hlt");
    }
}

int taskcharA = 0;
int taskcharB = 0;
int taskcharC = 0;
void task_a(int _argc, char **_argv)
{
    while (1) {
        putchar((taskcharA-8) % 800, 80, white, white, vinfo_global, ' ');
        putchar(taskcharA % 800, 80, white, red, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharA++;
    }
}

void task_b(int _argc, char **_argv)
{
    for (int i = 1; i < 100; i++) {
        putchar((taskcharB-8) % 800, 96, white, white, vinfo_global, ' ');
        putchar(taskcharB % 800, 96, white, blue, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharB += 2;
    }
}

void task_c(int _argc, char **_argv)
{
    while (1) {
        putchar((taskcharC-8) % 800, 112, white, white, vinfo_global, ' ');
        putchar(taskcharC % 800, 112, white, green, vinfo_global, ' ');
        asm volatile("hlt");
        taskcharC += 3;
    }
}
