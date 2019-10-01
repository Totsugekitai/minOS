#include <stdint.h>
#include <core/global_variables.h>
#include <types/boottypes.h>
#include <util/util.h>
#include <acpi/acpi.h>
#include <device/device.h>

const uint32_t PMTimerFreq = 3579545;
const uint32_t Max_Count = 0xffffffffu;
uint32_t *LVT_Timer = (uint32_t *)0xfee00320;
uint32_t *Initial_Count = (uint32_t *)0xfee00380;
uint32_t *Current_Count = (uint32_t *)0xfee00390;
uint32_t *Divide_Configuration = (uint32_t *)0xfee003e0;
uint64_t lapic_freq = 0;

void *get_xsdt_other_table(char *sig)
{
    struct XSDT *xsdt = (struct XSDT *)rsdp->xsdt_address;
    if (xsdt == 0) {
        puts_serial("failed to get XSDT\n");
    }
    uint32_t num_other_tables = xsdt->header.length - sizeof(struct XSDT_sys_desc_header) / sizeof(uint64_t);
    for (uint64_t i = 0; i < num_other_tables; i++) {
        if (strncmp(sig, ((struct XSDT_sys_desc_header *)(xsdt->other_tables[i]))->signature, 4) == 0) {
            return (void *)(xsdt->other_tables[i]);
        }
    }
    return (void *)0;
}

void wait_milliseconds(uint32_t *pm_timer_addr, uint32_t msec)
{
    puts_serial("pm_timer_addr: ");
    putnum_serial((uint64_t)pm_timer_addr);
    puts_serial("\n");
    
    const uint32_t start = io_in32((uint64_t)pm_timer_addr);
    
    puts_serial("start: ");
    putnum_serial((uint64_t)start);
    puts_serial("\n");
    
    uint32_t end = start + PMTimerFreq * msec / 1000;
    
    puts_serial("end: ");
    putnum_serial((uint64_t)end);
    puts_serial("\n");
    
    uint32_t k = 0;

    puts_serial("after if\n");
    
    while ((k = io_in32((uint64_t)pm_timer_addr)) > end) {
        //putnum_serial(k);
        //puts_serial("\n");
    }
    
    puts_serial("after while\n");
}

uint64_t measure_freq(uint32_t *pm_timer_addr)
{
    *Divide_Configuration = 0xb;
    *LVT_Timer = (0x1 << 16) | 0x20; // IDT 0x20に割り込み
    puts_serial("before wait_milliseconds\n");
    wait_milliseconds(pm_timer_addr, 100);
    puts_serial("after wait_milliseconds\n");
    uint32_t end_count = *Current_Count;
    return (uint64_t)((Max_Count - end_count) * 10);
}

void init_local_APIC(void)
{
    puts_serial("start init_local_APIC\n");
    
    struct FADT *fadt = (struct FADT *)get_xsdt_other_table("FACP");
    if (fadt == 0) {
        puts_serial("failed to get FADT\n");
        return;
    }
    for (int i = 0; i < 4; i++) {
        putchar(300 + i * 8, 300, black, white, vinfo_global, fadt->header.signature[i]);
    }
    puts_serial("FADT: "); // debug
    putnum_serial((uint64_t)fadt);
    puts_serial("\n");
    
    uint32_t *PM_TMR_BLK = (uint32_t *)(fadt->PM_TMR_BLK & 0xffffffffffffffffull);
    
    puts_serial("get PM_TMR_BLK\n");
    puts_serial("PM_TMR_BLK: ");
    putnum_serial((uint64_t)PM_TMR_BLK);
    puts_serial("\n");
    
    fadt->flags |= 0x100; // タイマのカウンタを32bitにする
    
    puts_serial("fadt->flags: ");
    putnum_serial((uint64_t)(fadt->flags));
    puts_serial("\n");
    
    lapic_freq = measure_freq(PM_TMR_BLK);
    
    puts_serial("lapic_freq: ");
    putnum_serial((uint64_t)lapic_freq);
    puts_serial("\n");
    
    *LVT_Timer = 0x20020; // 周期モード,割り込みマスクなし,IDT 0x20に割り込み
    
    puts_serial("*LVT_Timer: ");
    putnum_serial((uint64_t)*LVT_Timer);
    puts_serial("\n");
    
    *Initial_Count = lapic_freq / 1000; // 1msecごとにタイマ割り込み
}

