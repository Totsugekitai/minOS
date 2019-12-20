#include <stdint.h>
#include <device/device.h>
#include <device/sata.h>

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

// AHCI Address Base
// It is defined at pci.c
extern uint64_t *abar;

#define PORT0_OFFSET ((abar)+(0x100))

void print_hba_memory_register(void)
{
    struct HBA_MEM *hba_memreg = (struct HBA_MEM *)abar;
    puts_serial("HBA Memory Register\n");

    puts_serial("status of Generic Host Control\n");
    put_str_num_serial("cap: ", (uint64_t)hba_memreg->cap);
    put_str_num_serial("ghc: ", (uint64_t)hba_memreg->ghc);
    put_str_num_serial("is: ", (uint64_t)hba_memreg->is);
    put_str_num_serial("pi: ", (uint64_t)hba_memreg->pi);
    put_str_num_serial("vs: ", (uint64_t)hba_memreg->vs);
    put_str_num_serial("ccc_ctl: ", (uint64_t)hba_memreg->ccc_ctl);
    put_str_num_serial("ccc_pts: ", (uint64_t)hba_memreg->ccc_pts);
    put_str_num_serial("em_loc: ", (uint64_t)hba_memreg->em_loc);
    put_str_num_serial("em_ctl: ", (uint64_t)hba_memreg->em_ctl);
    put_str_num_serial("cap2: ", (uint64_t)hba_memreg->cap2);
    put_str_num_serial("bohc: ", (uint64_t)hba_memreg->bohc);

    struct HBA_PORT port0 = hba_memreg->ports[0];
    puts_serial("status of HBA Port 0\n");
    put_str_num_serial("clb: ", (uint64_t)port0.clb);
    put_str_num_serial("clbu: ", (uint64_t)port0.clbu);
    put_str_num_serial("fb: ", (uint64_t)port0.fb);
    put_str_num_serial("fbu: ", (uint64_t)port0.fbu);
    put_str_num_serial("is: ", (uint64_t)port0.is);
    put_str_num_serial("ie: ", (uint64_t)port0.ie);
    put_str_num_serial("cmd: ", (uint64_t)port0.cmd);
    put_str_num_serial("tfd: ", (uint64_t)port0.tfd);
    put_str_num_serial("sig: ", (uint64_t)port0.sig);
    put_str_num_serial("ssts: ", (uint64_t)port0.ssts);
    put_str_num_serial("sctl: ", (uint64_t)port0.sctl);
    put_str_num_serial("serr: ", (uint64_t)port0.serr);
    put_str_num_serial("sact: ", (uint64_t)port0.sact);
    put_str_num_serial("ci: ", (uint64_t)port0.ci);
    put_str_num_serial("sntf: ", (uint64_t)port0.sntf);
    put_str_num_serial("fbs: ", (uint64_t)port0.fbs);
}

void check_ahci(void)
{
    print_hba_memory_register();
}