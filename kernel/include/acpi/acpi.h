#pragma once
#include <stdint.h>

#define PCAT_COMPAT     0x1

struct sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct xsd_table {
    struct sdt_header header;
    struct sdt_header *entry[0];
} __attribute__((packed));

struct madt {
    struct sdt_header sdth;
    uint32_t local_int_controller_addr;
    uint32_t flags;
    uint8_t structure[];
};

struct hpet_table {
    struct sdt_header;
    uint32_t event_timer_block_id;

} __attribute__((packed));

void *get_sdt(char *sig);
extern void disabling_PIC(void);
