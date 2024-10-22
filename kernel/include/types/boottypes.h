#pragma once
#include <stdint.h>
struct pix_format {
    uint8_t bl;
    uint8_t gr;
    uint8_t re;
    uint8_t _reserved;
};

struct video_info {
    struct pix_format *fb;
    uint64_t fb_size;
    uint32_t x_axis;
    uint32_t y_axis;
    uint32_t ppsl;
};

struct RSDP {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t ex_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct memory_descriptor {
    uint32_t type;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t number_of_pages;
    uint64_t attribute;
};

struct bootinfo {
    struct video_info vinfo;
    struct RSDP *rsdp;
    struct memory_descriptor *memory_descriptor;
    uint64_t mmapsize;
    uint64_t memdescsize; 
};
