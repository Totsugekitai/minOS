#include <stdint.h>
#include <types/boottypes.h>
#include <util/util.h>
#include <acpi/acpi.h>

#define PMTimerFreq     (uint32_t)3579545

struct RSDP *rsdp;
const uint32_t Max_Count = 0xffffffffu;
uint32_t *LVT_Timer = 0xfee00320;
uint32_t *Initial_Count = 0xfee00380;
uint32_t *Current_Count = 0xfee00390;
uint32_t *Divide_Configuration = 0xfee003e0;

void *get_xsdt_other_table(char *sig)
{
    struct XSDT *xsdt = (struct XSDT *)rsdp->xsdt_address;
    uint32_t num_other_tables = xsdt->header.length - sizeof(struct XSDT_sys_desc_header) / sizeof(uint64_t);
    for (uint64_t i = 0; i < num_other_tables; i++) {
        if (strncmp(sig, ((struct XSDT_sys_desc_header *)(xsdt->other_tables[i]))->signature, 4)) {
            return (void *)(xsdt->other_tables[i]);
        }
    }
    return (void *)0;
}

void wait_milliseconds(uint32_t *pm_timer_addr, uint32_t msec)
{
    const uint32_t start = io_in32((uint32_t)pm_timer_addr);
    uint32_t end = start + PMTimerFreq * msec / 1000;
    if (end < start) {
        while (io_in32((uint32_t)pm_timer_addr) >= start);
    }
    while (io_in32((uint32_t)pm_timer_addr) < end);
}

void init_local_APIC(void)
{
    struct FADT *fadt = (struct FADT *)get_xsdt_other_table("FACP");
    uint32_t *PM_TMR_BLK = (uint32_t *)fadt->PM_TMR_BLK;
    fadt->flags |= 0x10; // タイマのカウンタを32bitにする
}

