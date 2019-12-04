#include <stdint.h>
#include <util/util.h>
#include <device/device.h>

#define PCI_CONFIG_ADDRESS 0xcf8
#define PCI_CONFIG_DATA 0xcfc

uint16_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t bus_number = (uint32_t)bus;
    uint32_t device_number = (uint32_t)device;
    uint32_t func_number = (uint32_t)func;

    /* create configuration address */
    uint32_t address = (uint32_t)((bus_number << 16) | (device_number << 11) |
                                  (func_number << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */
    io_out32(PCI_CONFIG_ADDRESS, address);

    /* read in the data */
    return (uint8_t)((io_in32(PCI_CONFIG_DATA) >> ((offset % 4) * 8)) & 0xffff);
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t bus_number = (uint32_t)bus;
    uint32_t device_number = (uint32_t)device;
    uint32_t func_number = (uint32_t)func;

    /* create configuration address */
    uint32_t address = (uint32_t)((bus_number << 16) | (device_number << 11) |
                                  (func_number << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */
    io_out32(PCI_CONFIG_ADDRESS, address);

    /* read in the data */
    return (uint16_t)((io_in32(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}

struct pci_config_space pci_check_parameters(uint8_t bus, uint8_t slot, uint8_t func)
{
    struct pci_config_space config;
    /* try and read the first configuration register */
    /* vendors that == 0xffff, it must be a non-existent device. */
    if ((config.vendor = pci_config_read_word(bus, slot, func, 0x00)) != 0xffff) {
        config.device = pci_config_read_word(bus, slot, func, 0x02);
        // config.command = pci_config_read_word(bus, slot, func, 0x04);
        config.status = pci_config_read_word(bus, slot, func, 0x06);
        // config.revision_id = pci_config_read_byte(bus, slot, func, 0x08);
        config.prog_if = pci_config_read_byte(bus, slot, func, 0x09);
        config.subclass = pci_config_read_byte(bus, slot, func, 0x0a);
        config.class_code = pci_config_read_byte(bus, slot, func, 0x0b);
        // config.cache_line_size = pci_config_read_byte(bus, slot, func, 0x0c);
        // config.latency_timer = pci_config_read_byte(bus, slot, func, 0x0d);
        config.header_type = pci_config_read_byte(bus, slot, func, 0x0e);
        // config.bist = pci_config_read_byte(bus, slot, func, 0x0f);

        config.capabilities_pointer = pci_config_read_byte(bus, slot, func, 0x34);

        // config.interrupt_line = pci_config_read_byte(bus, slot, func, 0x3c);
        // config.interrupt_pin = pci_config_read_byte(bus, slot, func, 0x3d);
        config.address_map_register = pci_config_read_word(bus, slot, func, 0x90);
    }
    return config;
}

void check_all_buses(void)
{
    struct pci_config_space config;
    for (int bus = 0; bus < 256; bus++) {
        for (int device = 0; device < 32; device++) {
            for (int func = 0; func < 8; func++) {
                config = pci_check_parameters(bus, device, func);
                if (config.vendor != 0xffff) {
                    put_str_num_serial("bus: ", bus);
                    put_str_num_serial("device: ", device);
                    put_str_num_serial("function: ", func);
                    put_str_num_serial("vendor: ", config.vendor);
                    put_str_num_serial("device: ", config.device);
                    put_str_num_serial("status: ", config.status);
                    put_str_num_serial("prog_if: ", config.prog_if);
                    put_str_num_serial("subclass: ", config.subclass);
                    put_str_num_serial("class_code: ", config.class_code);
                    put_str_num_serial("header_type: ", config.header_type);
                    put_str_num_serial("capabilities_pointer: ", config.capabilities_pointer);
                    put_str_num_serial("address_map_register: ", config.address_map_register);
                    puts_serial("\n");
                }
            }
        }
    }
}