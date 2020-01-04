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
};

struct pci_config_space pci_check_parameters(uint8_t bus, uint8_t slot, uint8_t func);
void check_all_buses(void);
void print_device_info(int bus, int device, int func, struct pci_config_space config);
uint64_t *get_ahci_base_address(int bus, int device, int func);
