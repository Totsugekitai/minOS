#include <stdint.h>
#define PML4_NUM        1
#define PD_NUM          16
#define TABLE_SIZE      512
#define PAGE_SIZE_2M    (uint64_t)0x00200000 // 2MiB
#define PAGE_SIZE_1G    512 * PAGE_SIZE_2M // 1GiB

void create_pgtable(uint64_t *pml4_addr,
        uint64_t *pdp_addr, uint64_t *pd_addr)
{
    int i;
    // ゼロフィル
    for (i = 0; i < TABLE_SIZE; i++) {
        pml4_addr[i] = 0;
        pdp_addr[i] = 0;
    }
    for (i = 0; i < TABLE_SIZE * PD_NUM; i++) {
        pd_addr[i] = 0;
    }

    // PDのエントリをフォーマット
    for (i = 0; i < TABLE_SIZE * PD_NUM; i++) {
        pd_addr[i] = ((uint64_t)(i * PAGE_SIZE_2M) | 0x083);
    }

    // PDPのエントリにPDを登録
    // pdp[0]に&pd[0]を、pdp[1]に&pd[1]を...
    for (i = 0; i < PD_NUM; i++) {
        pdp_addr[i] = ((uint64_t)(&pd_addr[i * TABLE_SIZE]) | 0x003);
    }

    // PML4のエントリにPDPを登録
    // pml4[0]に&pdp[0]を登録するだけ
    pml4_addr[0] = ((uint64_t)(&pdp_addr[0]) | 0x003);

    return;
}

