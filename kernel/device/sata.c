#include <stdint.h>
#include <device/device.h>

#define SATA_SIG_ATA 0x00000101   // SATA drive
#define SATA_SIG_ATAPI 0xEB140101 // SATAPI drive
#define SATA_SIG_SEMB 0xC33C0101  // Enclosure management bridge
#define SATA_SIG_PM 0x96690101    // Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

void probe_port(struct hba_mem_regs *reg)
{
    uint32_t port_impl = reg->generic_host_ctl.port_impl;
    for (int i = 0; i < 32; i++) {
        if (port_impl & 1) {
            int dt = check_type(&reg->ports[i]);
            if (dt == AHCI_DEV_SATA) {
                put_str_num_serial("SATA drive found at port: ", i);
            } else if (dt == AHCI_DEV_SATAPI) {
                put_str_num_serial("SATAPI drive found at port: ", i);
            } else if (dt == AHCI_DEV_SEMB) {
                put_str_num_serial("SEMB drive found at port: ", i);
            } else if (dt == AHCI_DEV_PM) {
                put_str_num_serial("PM drive found at port: ", i);
            } else {
                put_str_num_serial("No drive found at port: ", i);
            }
        }
    }
}

static int check_type(struct port_reg *port)
{
    uint32_t ssts = port->ssts;

    uint8_t ipm = (ssts >> 8) & 0x0f;
    uint8_t det = ssts & 0x0f;

    if (det != HBA_PORT_DET_PRESENT) {
        return AHCI_DEV_NULL;
    }
    if (ipm != HBA_PORT_IPM_ACTIVE) {
        return AHCI_DEV_NULL;
    }

    switch (port->sig) {
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
        }
}
