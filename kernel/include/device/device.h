#pragma once
#include <stdint.h>

/* PS/2 keyboard and serial */
extern uint8_t read_kbd_signal(void);
extern uint8_t read_kbd_status(void);
void init_serial(void);
uint32_t serial_received(void);
void wait_serial_input(void);
uint8_t receive_serial_input(void);
uint8_t read_serial(void);
uint8_t serial_thr_empty(void);
void write_serial(uint8_t c);
void puts_serial(const char *s);
void putnum_serial(uint64_t num);
void put_str_num_serial(const char *s, uint64_t n);
uint8_t map_scan_to_ascii_set1(uint8_t scan, uint8_t *shift);
//uint8_t map_scan_to_ascii_set2(uint8_t scan, uint8_t *shift);
uint8_t ps2_received(void);

/* PCI */
struct pci_config_space {
    uint16_t vendor;
    uint16_t device;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;

    uint8_t capabilities_pointer;

    uint8_t interrupt_line;
    uint8_t interrupt_pin;

    uint16_t address_map_register;
};
struct pci_config_space pci_check_parameters(uint8_t bus, uint8_t slot, uint8_t func);
void check_all_buses(void);

/* Serial-ATA */
struct generic_host_ctl;
struct port_reg;
struct cmd_header;
struct cmd_list;
struct phy_region_desc_tbl;

struct hba_mem_regs {
    struct generic_host_ctl generic_host_ctl;
    uint8_t reserved[0xa0-0x2c];
    uint8_t vsrs[0x100-0xa0];
    struct port_reg ports[32];
};

struct generic_host_ctl {
    uint32_t hba_cap;
    uint32_t globl_hba_ctl;
    uint32_t int_stat;
    uint32_t port_impl;
    uint32_t version;
    uint32_t ccc_ctl;
    uint32_t ccc_ports;
    uint32_t em_loc;
    uint32_t em_ctl;
    uint32_t hba_cap2;
    uint32_t bohc;
};

struct port_reg {
    uint32_t clb;
    uint32_t clbu;
    uint32_t fb;
    uint32_t fbu;
    uint32_t is;
    uint32_t ie;
    uint32_t cmd;
    uint32_t reserved0;
    uint32_t tfd;
    uint32_t sig;
    uint32_t ssts;
    uint32_t serr;
    uint32_t sact;
    uint32_t ci;
    uint32_t sntf;
    uint32_t fbs;
    uint32_t reserved1[11];
    uint32_t vs[4];
};

struct cmd_list {
    struct cmd_header headers[32];
};

struct cmd_header {
    // DW0
    uint32_t flags;
    // DW1
    uint32_t prdbc;
    // DW2-3
    uint32_t ctba0;
    uint32_t ctba_u0;
    // DW4-7
    uint32_t reserved[4];
};

struct cmd_tbl {
    // 0x00
    uint8_t cfis[64];
    // 0x40
    uint8_t acmd[16];
    // 0x50
    uint8_t reserved[48];
    // 0x80
    struct phy_region_desc_tbl prdt_entry[1];
};

struct phy_region_desc_tbl {
    uint32_t dba;
    uint32_t dbau;
    uint32_t reserved0;
    uint32_t flags;
};