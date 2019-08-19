#pragma once
#include <stdint.h>
#include <graphics/graphics.h>
#define PCAT_COMPAT     0x1

struct XSDT_sys_desc_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct XSDT {
    struct XSDT_sys_desc_header header;
    uint64_t other_tables[];
} __attribute__((packed));

struct FADT {
    struct XSDT_sys_desc_header header;
    uint64_t other_params1[5];
    uint32_t PM_TMR_BLK;
    uint32_t other_params2[9];
    uint32_t flags;
    uint32_t other_params3[39];
};

void *get_xsdt_other_table(char *sig);
void init_local_APIC(void);
