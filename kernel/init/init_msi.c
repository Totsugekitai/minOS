#include <stdint.h>
#include <util/util.h>

uint8_t *get_pci_capability_pointer(void)
{
    // PCI Configuration Registerを読み出し
    uint32_t config_address_register = io_in32(0xcf8);
    // Enable bitを1にして書き込み
    config_address_register |= 80000000;
    io_out32(0xcf8, config_address_register);
    
    uint8_t *capability_pointer;
    return capability_pointer;
}

void init_msi(void)
{
    uint8_t *capability_pointer = get_pci_capability_pointer();
}

