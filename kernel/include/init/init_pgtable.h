#define PML4_NUM        1
#define PD_NUM          4
#define TABLE_SIZE      512
#define PAGE_SIZE_2M    (long)0x00200000 // 2MiB
#define PAGE_SIZE_1G    512 * PAGE_SIZE_2M // 1GiB

#define PAGE_START      (long *)0x00000000

long pml4[TABLE_SIZE], pdp[TABLE_SIZE], pd[PD_NUM][TABLE_SIZE];
