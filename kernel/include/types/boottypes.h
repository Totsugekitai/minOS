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

struct acpi_table {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtaddr;
    uint32_t length;
    uint64_t xsdtaddr;
    uint8_t checksum2;
    uint8_t reserved[3];
} __attribute__((packed));

struct bootinfo {
    struct video_info vinfo;
    struct acpi_table *acpi_info;
};
