#include <stdint.h>
#include <util/util.h>
#include <mm/memory.h>
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

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define SIZE_8KB (8 * 1024 - 1)

// AHCI Address Base
// It is defined at pci.c
extern uint64_t *abar;

void print_hba_memory_register(void)
{
    HBA_MEM_REG *hba_memreg = (HBA_MEM_REG *)abar;
    //puts_serial("HBA Memory Register\n");

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
    puts_serial("\n");
}

void print_port_status(HBA_PORT *port)
{
    puts_serial("status of HBA Port\n");
    put_str_num_serial("clb: ", (uint64_t)port->clb);
    put_str_num_serial("clbu: ", (uint64_t)port->clbu);
    put_str_num_serial("fb: ", (uint64_t)port->fb);
    put_str_num_serial("fbu: ", (uint64_t)port->fbu);
    put_str_num_serial("is: ", (uint64_t)port->is);
    put_str_num_serial("ie: ", (uint64_t)port->ie);
    put_str_num_serial("cmd: ", (uint64_t)port->cmd);
    put_str_num_serial("tfd: ", (uint64_t)port->tfd);
    put_str_num_serial("sig: ", (uint64_t)port->sig);
    put_str_num_serial("ssts: ", (uint64_t)port->ssts);
    put_str_num_serial("sctl: ", (uint64_t)port->sctl);
    put_str_num_serial("serr: ", (uint64_t)port->serr);
    put_str_num_serial("sact: ", (uint64_t)port->sact);
    put_str_num_serial("ci: ", (uint64_t)port->ci);
    put_str_num_serial("sntf: ", (uint64_t)port->sntf);
    put_str_num_serial("fbs: ", (uint64_t)port->fbs);
    puts_serial("\n");
}

/* ----------------------------------------------------------------------------
 * AHCI minimal initialization utility procedures and functions
 * ----------------------------------------------------------------------------
 */

static inline void hba_reset(void)
{
    HBA_MEM_REG *memreg = (HBA_MEM_REG *)abar;
    // GHC.AE = 1 and GHC.HR = 1 to reset HBA
    memreg->ghc = 0x80000000;
    memreg->ghc |= 0x00000001;
    // during HR = 1, polling
    while (memreg->ghc & 0x1) {
        puts_serial(".");
        asm volatile("hlt");
    }
    puts_serial("\n");
}

// if PxCMD.ST, PxCMD.CR, and PxCMD.FRE is clear, the port is idle.
static uint32_t probe_idle_port(uint32_t pi)
{
    HBA_MEM_REG *memreg = (HBA_MEM_REG *)abar;
    uint32_t pidle = 0;
    for (int i = 0; i < 32; i++) {
        if (pi >> i) {
            if ((memreg->ports[i].cmd & 0x811) == 0) {
                pidle |= 1 << i;
            } else {
                put_str_num_serial("port is busy - PxCMD: ", (uint64_t)memreg->ports[i].cmd);
            }
        }
    }
    return pidle;
}

#define CMD_LIST_BASE 0x10000000    // size: 0x20 * 32 * 32 = 0x8000
#define RCVD_FIS_BASE 0x10008000    // size: 0x100 * 32     = 0x2000
static inline void alloc_mem_for_ports(uint32_t pi_list, uint16_t slot_num)
{
    memset((void *)CMD_LIST_BASE, 0, 0x8000 + 0x2000);
    HBA_PORT *ports = (HBA_PORT *)&(((HBA_MEM_REG *)abar)->ports[0]);
    for (int i = 0; i < 32; i++) {
        if (pi_list >> i) {
            ports[i].clb = CMD_LIST_BASE + sizeof(HBA_CMD_HEADER) * 32 * i;
            ports[i].fb = RCVD_FIS_BASE + sizeof(RCVD_FIS) * i;
            ports[i].cmd |= 0x10;  // PxCMD.FRE is set to 1
        }
    }
}

static inline void clear_ports_serr(uint32_t pi_list)
{
    HBA_PORT *ports = (HBA_PORT *)&(((HBA_MEM_REG *)abar)->ports[0]);
    for (int i = 0; i < 32; i++) {
        if (pi_list >> i) {
            ports[i].serr |= 0x7ff0f03;    // clear by writing 1s to each bit
            while (ports[i].serr)
                asm volatile("hlt");
        }
    }
}

static inline void enable_ahci_interrupt(uint32_t pi_list)
{
    // first, PxIS are cleared to 0
    HBA_MEM_REG *ghc = (HBA_MEM_REG *)abar;
    HBA_PORT *ports = (HBA_PORT *)&(ghc->ports[0]);
    for (int i = 0; i < 32; i++) {
        if (pi_list >> i) {
            //ports[i].is &= ~(ports[i].is);    // clear by writing 1s to each bit
            ports[i].is = 0xffffffff; // clear pending interrupt bits
            while (ports[i].is) {
                asm volatile("hlt");
                put_str_num_serial("PxIS clear cannot be finished: ", (uint64_t)ports[i].is);
            }
        }
    }
    // second, IS.IPS is cleared to 0
    ghc->is &= ~(ghc->is);
    while (ghc->is) {
        asm volatile("hlt");
        puts_serial("GHC.IS.IPS clear cannot be finished.\n");
    }
    // enable PxIE bit
    for (int i = 0; i < 32; i++) {
        if (pi_list >> i) {
            ports[i].ie = 0xfdc000ff;  // Interrupt Enable all set
        }
    }
    // set GHC.IE to 1
    ghc->ghc |= 0x2;
}

/*
 * Initialization procedure
 * This is MINIMAL initialization.
 * (10.1.2 System Software Specific Initialization)
 * */
static inline void ahci_init(void)
{
    puts_serial("AHCI initialization start.\n");
    // initial HBA reset
    hba_reset();
    puts_serial("HBA reset end.\n");

    HBA_MEM_REG *memreg = (HBA_MEM_REG *)abar;
    // step 1: GHC.AE = 1
    memreg->ghc |= 0x80000000;
    puts_serial("AHCI init step 1 end.\n");
    // step 2: determine implemented port
    uint32_t pi = memreg->pi;
    puts_serial("AHCI init step 2 end.\n");
    // step 3: ensure that the controller is not running state
    // if PxCMD.ST, PxCMD.CR, and PxCMD.FRE is clear, the port is idle.
    uint32_t pidle = probe_idle_port(pi);
    while (pi != pidle) {
        put_str_num_serial("Implement port is: ", pi);
        put_str_num_serial("Implement port is not idle: ", pidle);
        asm volatile("hlt");
    }
    puts_serial("AHCI init step 3 end.\n");
    // step 4: determine how many command slots the HBA supports
    uint16_t slot_num = (uint16_t)((memreg->cap >> 8) & 0x1f);
    puts_serial("AHCI init step 4 end.\n");
    // step 5: allocate memory for implemented ports
    // required params: PxCLB and PxFB
    alloc_mem_for_ports(pi, slot_num);
    puts_serial("AHCI init step 5 end.\n");
    // step 6: clear PxSERR
    clear_ports_serr(pi);
    puts_serial("AHCI init step 6 end.\n");
    // step 7: enable interrupt
    enable_ahci_interrupt(pi);
    puts_serial("AHCI init step 7 end.\n");
    puts_serial("All of AHCI initialization end.\n");
}

/* ----------------------------------------------------------------------------
 * AHCI Read/Write utility procedures and functions
 * ----------------------------------------------------------------------------
 */

static int find_free_cmdslot(HBA_PORT *port)
{
    uint32_t slots = (port->ci | port->sact);
    for (int i = 0; i < 32; i++) {
        if ((slots & 1) == 0) {
            return i;
        }
        slots >>= 1;
    }
    puts_serial("Cannot find free command list entry\n");
    return -1;
}

#define CMD_TBL_BASE 0x10010000
static inline void build_cmd_table(CMD_PARAMS *params, uint64_t *table_addr)
{
    memset((void *)table_addr, 0, 0x80 + 16 * 65536); // zero clear
    HBA_CMD_TBL *table = (HBA_CMD_TBL *)table_addr;

    // build CFIS
    if (params->fis_type == 0x27) { // if Register H2D
        FIS_REG_H2D *h2dfis = (FIS_REG_H2D *)table_addr;
        h2dfis->fis_type = 0x27;    // H2D FIS type magic number
        h2dfis->c = 1;              // This is command
        // command type is referenced in ATA command set
        h2dfis->command = params->cmd_type;
        // device
        h2dfis->device = 0xe0;
        // LBA
        h2dfis->lba0 = (uint8_t)(((uint64_t)(params->lba) >>  0) & 0xff);
        h2dfis->lba1 = (uint8_t)(((uint64_t)(params->lba) >>  8) & 0xff);
        h2dfis->lba2 = (uint8_t)(((uint64_t)(params->lba) >> 16) & 0xff);
        h2dfis->lba3 = (uint8_t)(((uint64_t)(params->lba) >> 24) & 0xff);
        h2dfis->lba4 = (uint8_t)(((uint64_t)(params->lba) >> 32) & 0xff);
        h2dfis->lba5 = (uint8_t)(((uint64_t)(params->lba) >> 40) & 0xff);
        // block count
        h2dfis->countl = (uint8_t)((params->count >> 0) & 0xff);
        h2dfis->counth = (uint8_t)((params->count >> 8) & 0xff);
    } else {
        // TODO: other FIS is not implemented.
    }

    // build PRD Table
    // 8 KB (16 sectors) per PRD Table
    // 1 sectors = 512 KB
    uint16_t count = params->count;
    int prdtl = (int)((params->count - 1) >> 4) + 1;
    uint8_t *buf = (uint8_t *)params->dba;
    int i;
    for (i = 0; i < prdtl - 1; i++) {
        table->prdt_entry[i].dba  = (uint32_t)buf;
        table->prdt_entry[i].dbau = 0;
        table->prdt_entry[i].dbc = 8 * 1024 - 1;
        table->prdt_entry[i].i = 1; // notify interrupt
        buf += 8 * 1024; // 8K bytes
        count -= 16; // 16 sectors
    }

    // Last entry
    table->prdt_entry[i].dba = (uint32_t)buf;
    table->prdt_entry[i].dbc = (count << 9) - 1;
    table->prdt_entry[i].i = 1;
}

static inline void build_cmdheader(HBA_PORT *port, int slot, CMD_PARAMS *params)
{
    HBA_CMD_HEADER *cmd_list = ((HBA_CMD_HEADER *)port->clb + slot);
    memset((void *)cmd_list, 0, 0x400);
    cmd_list->ctba = (uint32_t)CMD_TBL_BASE;
    cmd_list->ctbau = 0;
    cmd_list->prdtl = (uint16_t)(((params->count - 1) >> 4) + 1);
    cmd_list->cfl = params->cfis_len;
    cmd_list->w = params->w;
}

static inline void notify_cmd_is_active(HBA_PORT *port, int slot)
{
    port->ci |= 1 << slot;
}

static inline void build_command(HBA_PORT *port, CMD_PARAMS *params)
{
    int slot = find_free_cmdslot(port);
    // step 1:
    // build a command FIS in system memory at location PxCLB[CH(pFreeSlot)]:CFIS with the command type.
    build_cmd_table(params, (uint64_t *)CMD_TBL_BASE);
    // step 2:
    // build a command header at PxCLB[CH(pFreeSlot)].
    build_cmdheader(port, slot, params);
    // step 3:
    // set PxCI.CI(pFreeSlot) to indicate to the HBA that a command is active.
    notify_cmd_is_active(port, slot);

    puts_serial("build command is over\n");
    print_port_status(port);
}

static inline void wait_interrupt(HBA_PORT *port)
{
    puts_serial("while waiting interrupt\n");
    while (port->is == 0) {
        asm volatile("hlt");
    }
    puts_serial("interrupt comes\n");
    print_port_status(port);
}

static inline void clear_pxis(HBA_PORT *port)
{
    port->is |= port->is;
    puts_serial("while clear PxIS\n");
    while (port->is) {
        asm volatile("hlt");
    }
    puts_serial("clearing PxIS is over\n");
    print_port_status(port);
}

static inline void clear_ghc_is(int portno)
{
    HBA_MEM_REG *ghc = (HBA_MEM_REG *)abar;
    ghc->is |= 1 << portno;
    puts_serial("while clear IS.IPS\n");
    while (ghc->is) {
        asm volatile("hlt");
    }
    puts_serial("clearing IS.IPS is finished\n");
}

int ahci_read(HBA_PORT *port, int portno, uint64_t start, uint16_t count, uint16_t *buf)
{
    CMD_PARAMS params;
    params.fis_type = 0x27;
    params.cmd_type = READ_DMA_EXT;
    params.cfis_len = 5;
    params.lba = (uint64_t *)start;
    params.count = count;
    params.dba = (uint64_t *)buf;
    params.w = 0;
    build_command(port, &params);
    wait_interrupt(port);
    clear_pxis(port);
    clear_ghc_is(portno);
    return 1;
}

// Check device type
static int check_type(HBA_PORT *port)
{
    uint32_t ssts = port->ssts;

    uint8_t ipm = (ssts >> 8) & 0x0F;
    uint8_t det = ssts & 0x0F;

    if (det != HBA_PORT_DET_PRESENT) { // Check drive status
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

void probe_port(struct port_implemented *p_impl)
{
    // Search disk in implemented ports
    HBA_MEM_REG *hba_memreg = (HBA_MEM_REG *)abar;
    uint32_t pi = hba_memreg->pi;
    int i = 0;
    while (i < 32) {
        if (pi & 1) {
            // int dt = check_type(&hba_memreg->ports[i]);
            int dt = check_type(&hba_memreg->ports[i]);
            if (dt == AHCI_DEV_SATA) {
                put_str_num_serial("SATA drive found at port ", i);
                p_impl->sata_bit |= 1 << i;
            } else if (dt == AHCI_DEV_SATAPI) {
                put_str_num_serial("SATAPI drive found at port ", i);
                p_impl->atapi_bit |= 1 << i;
            } else if (dt == AHCI_DEV_SEMB) {
                put_str_num_serial("SEMB drive found at port ", i);
                p_impl->semb_bit |= 1 << i;
            } else if (dt == AHCI_DEV_PM) {
                put_str_num_serial("PM drive found at port ", i);
                p_impl->pm_bit |= 1 << i;
            } else {
                put_str_num_serial("No drive found at port ", i);
                p_impl->no_bit |= 1 << i;
            }
        }

        pi >>= 1;
        i++;
    }
}

static inline void stop_cmd(HBA_PORT *port)
{
    // clear ST
    port->cmd &= ~0x01;

    // wait until FR, CR are cleared
    while (1) {
        if (port->cmd & 0xc000) {
            continue;
        } else {
            break;
        }
    }

    // clear FRE
    port->cmd &= ~0x10;
}

static inline void start_cmd(HBA_PORT *port)
{
    // wait until CR is cleared
    while (port->cmd & 0x8000) { asm volatile("hlt"); }

    // set FRE and ST
    port->cmd |= 0x10;
    port->cmd |= 0x01;
}

static inline void port_rebase(HBA_PORT *port, int portno)
{
    stop_cmd(port);
    for (int i = 0; i < 0xe00; i++) {
        putnum_serial((uint64_t)i);
    }
    print_port_status(port);

    // Command List offset: 1K * portno
    // Command List entry size: 32
    // Command List entry max count: 32
    port->clb = CMD_LIST_BASE + (portno << 10);
    port->clbu = 0;
    memset((void *)(port->clb), 0, 1024);

    // FIS offset: 32K + 256 * portno
    // FIS entry size: 256byte per port
    port->fb = CMD_LIST_BASE + (32 << 10) + (portno << 8);
    port->fbu = 0;
    memset((void *)(port->fb), 0, 256);

    // Command Table offset: 40K + 8K * portno
    // Command Table size: 256 * 32 = 8K per port
    HBA_CMD_HEADER *cmd_header = (HBA_CMD_HEADER *)(port->clb);
    for (int i = 0; i < 32; i++) {
        cmd_header[i].prdtl = 8;    // 8 prdt entries per command table
                                    // 256 bytes per command table, 64+16+48+16*8
        // Command Table offset: 40K + 8K * portno + cmd_header_index * 256
        cmd_header[i].ctba = CMD_LIST_BASE + (40 << 10) + (portno << 13) + (i << 8);
        cmd_header[i].ctbau = 0;
        memset((void *)(cmd_header[i].ctba), 0, 256);
    }

    start_cmd(port);
}

// portを通じて
// count分のセクタを
// startl:starthが指すオフセットから読み取り、
// bufへ書き込む
int read(HBA_PORT *port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t *buf)
{
    //port->ie = 0xfdc000ff;  // Interrupt Enable all set
    //port->is = 0xffffffff; // clear pending interrupt bits
    //port->is = 0xfd8000af;  // Interrupt Status all reset
    //port->is = 0x0;  // Interrupt Status all reset
    while (1) {
        //put_str_num_serial("during IS reset: ", (uint64_t)port->is);
        print_port_status(port);
        if (port->is == 0) {
            break;
        }
    }

    int slot = find_free_cmdslot(port);
    if (slot == -1) {
        return 0;
    }

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)port->clb;
    cmdheader += slot; // 空きスロットへのアドレス
    cmdheader->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t); // Command FIS size
    cmdheader->w = 0;
    cmdheader->prdtl = (uint16_t)((count - 1) >> 4) + 1; // PRDT entries count

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)(cmdheader->ctba);
    // HBA_CMD_TBL末尾にはPRDT1エントリ分が含まれているのでそれと重複させないようにしつつ
    // command tableを0で初期化する
    memset((void *)cmdtbl, 0, sizeof(HBA_CMD_TBL) +
        (cmdheader->prdtl - 1) * sizeof(HBA_PRDT_ENTRY));

    // 8KB(16 sectors) per PRDT
    // PRDTエントリ群を設定
    int i;
    for (i = 0; i < cmdheader->prdtl - 1; i++) {
        cmdtbl->prdt_entry[i].dba = (uint32_t)buf;
        cmdtbl->prdt_entry[i].dbc = 8*1024-1;
        cmdtbl->prdt_entry[i].i = 1; // 割り込みを通知する設定
        buf += 4 * 1024; // 8K words
        count -= 16; // 16 sectors
    }

    // Last entry
    cmdtbl->prdt_entry[i].dba = (uint32_t)buf;
    cmdtbl->prdt_entry[i].dbc = (count << 9) - 1;
    cmdtbl->prdt_entry[i].i = 1;

    // Setup Command
    FIS_REG_H2D *cmdfis = (FIS_REG_H2D *)(&cmdtbl->cfis);

    cmdfis->fis_type = FIS_TYPE_REG_H2D;
    cmdfis->c = 1; // Commandであることを示す
    cmdfis->command = 0x25; // READ DMA EXTコマンドを表す数値

    cmdfis->lba0 = (uint8_t)startl;
    cmdfis->lba1 = (uint8_t)(startl >> 8);
    cmdfis->lba2 = (uint8_t)(startl >> 16);
    cmdfis->device = 1 << 6; // LBAモード

    cmdfis->lba3 = (uint8_t)(startl >> 24);
    cmdfis->lba4 = (uint8_t)starth;
    cmdfis->lba5 = (uint8_t)(starth >> 8);

    cmdfis->countl = count && 0xff;
    cmdfis->counth = (count >> 8) && 0xff;

    // portがbusy状態を脱するまで待機
    int spin = 0; // spin lock timeout counter
    while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000) {
        spin++;
    }
    put_str_num_serial("spin: ", (uint64_t)spin);
    if (spin == 1000000) {
        puts_serial("Port is hung\n");
        return 0;
    }
    port->ci = 1 << slot; // CIを立ててHBAに要請
    print_port_status(port);

    // コマンド実行が終了するまでループで待つ
    puts_serial("wait command read is finished\n");
    while (1) {
        //put_str_num_serial("port->ci: ", (uint64_t)port->ci);
        if ((port->ci & (1 << slot)) == 0) {
            puts_serial("command read finished\n");
            break;
        }
        //  port->isのbit 30がTask file error statusなのでそれを確認
        if (port->is & (1 << 30)) {
            puts_serial("Read disk error\n");
            return 0;
        }
        asm volatile("hlt");
    }

    // もう一回チェック
    if (port->is & (1 << 30)) {
        puts_serial("Read disk error at final check\n");
        return 0;
    }

    return 1;
}

void check_ahci(void)
{
    ahci_init();    // AHCI initialization
    print_hba_memory_register();

    HBA_PORT port = ((HBA_MEM_REG *)abar)->ports[0];
    print_port_status(&port);

    // while (1) {
    //     asm volatile("hlt");
    // }

    uint16_t buf[512] = {0};

    // port_rebase(&port, 0);
    print_port_status(&port);
    int err = ahci_read(&port, 0, 0x100, 1, buf);
    print_port_status(&port);

    if (!err) {
        puts_serial("AHCI read error!!!!!!!\n");
        return;
    }

    for (int i = 0; i < 256; i++) {
        putnum_serial((uint64_t)buf[i]);
    }
    while (1) {
        asm volatile("hlt");
    }
}

// 
// static uint32_t find_free_slot(HBA_PORT *port)
// {
//     return (uint32_t)(port->sact | port->ci);
// }
// 
// inline void init_ahci_controller(void)
// {
//     HBA_MEM_REG *ghc = (HBA_MEM_REG *)abar;
// 
//     // GHC.AE = 1 (AHCI enable)
//     // GHC.IE = 1 (Interrupt Enable)
//     ghc->ghc |= 0x80000002;
// 
//     // IS.IPS = 1 (reset Interrupt Pending Status)
//     ghc->is = 0xffffffff;
// 
//     // CCC_CTL.EN = 0 (reset Enable)
//     ghc->ccc_ctl &= 0x00000000;
// }
// 
// inline void init_ahci_port(HBA_PORT *port)
// {
//     port->clb = 0;
//     port->clbu = 0;
//     port->fb = 0;
//     port->fbu = 0;
// 
// 
//     port->is = 0xfd8000af;  // Interrupt Status all reset
//     port->ie = 0xfdc000ff;  // Interrupt Enable all set
// }
// 
// // 1 count = 512 byte
// static inline void build_regh2dfis(uint64_t *ctba, uint8_t cmd, uint64_t *lba, uint16_t count)
// {
//     FIS_REG_H2D *cfis = ctba;
//     cfis->fis_type = FIS_TYPE_REG_D2H;
//     cfis->c = 1; // command
//     cfis->command = cmd; // this magic number is defined at ATA command set
//     cfis->featurel = 0; // this field is expanded address
//     cfis->featureh = 0; // this field is expanded address
//     cfis->device = 1 << 6; // LBA mode
//     cfis->icc = 0; // this value is defined as time limit at qemu ahci.c command_table_init()
//     cfis->control = 0; // this value is defined 0 at qemu ahci.c command_table_init()
//     cfis->rsv0 = 0;
//     cfis->rsv1[0] = 0;
//     cfis->rsv1[1] = 0;
//     cfis->rsv1[2] = 0;
//     cfis->rsv1[3] = 0;
// 
//     cfis->countl = (uint8_t)(count & 0xff);
//     cfis->counth = (uint8_t)((count >> 8) & 0xff);
// 
//     cfis->lba0 = (uint8_t)(((uint64_t)lba >>  0) & 0xff);
//     cfis->lba1 = (uint8_t)(((uint64_t)lba >>  8) & 0xff);
//     cfis->lba2 = (uint8_t)(((uint64_t)lba >> 16) & 0xff);
//     cfis->lba3 = (uint8_t)(((uint64_t)lba >> 24) & 0xff);
//     cfis->lba4 = (uint8_t)(((uint64_t)lba >> 32) & 0xff);
//     cfis->lba5 = (uint8_t)(((uint64_t)lba >> 40) & 0xff);
// 
//     // flush previous data tail
//     uint32_t *blank_space = (uint32_t *)((char*)ctba + sizeof(FIS_REG_H2D));
//     for (int i = 0; i < 3; i++) {
//         blank_space[i] = 0;
//     }
// }
// 
// #define PRDT_OFFSET 0x80
// #define FOUR_MB 0x3fffff // 4MiB
// 
// static inline void build_prdt(CFIS *ctba, uint64_t *dba, uint32_t len)
// {
//     HBA_PRDT_ENTRY *prdt = (HBA_PRDT_ENTRY *)((char *)ctba + PRDT_OFFSET);
//     int chunk_num = len / FOUR_MB;
//     if (len % FOUR_MB != 0) {
//         chunk_num++;
//     }
//     for (int i = 0; i < chunk_num; i++) {
//         if (i + 1 == chunk_num) { // chunk length is not aligned as 4MiB
//             prdt[i]->dba = (uint32_t)((uint32_t)dba & 0xfffffffe); // data aligned as word
//             prdt[i]->dbau = (uint32_t)((uint32_t)dba >> 32);
//             prdt[i]->rsv0 = 0;
//             prdt[i]->dbc = len % FOUR_MB;
//             prdt[i]->rsv1 = 0;
//             prdt[i]->i = 0;
//         } else {                  // chunk length is aligned as 4MiB
//             prdt[i]->dba = (uint32_t)((uint32_t)dba & 0xfffffffe); // data aligned as word
//             prdt[i]->dbau = (uint32_t)((uint32_t)dba >> 32);
//             prdt[i]->rsv0 = 0;
//             prdt[i]->dbc = FOUR_MB;
//             prdt[i]->rsv1 = 0;
//             prdt[i]->i = 0;
//         }
//     }
// }
// 
// static inline void build_command_header(HBA_CMD_HEADER *clb, uint16_t prdtl, uint8_t write, uint8_t atapi, uint8_t cfl, uint64_t *ctba)
// {
//     HBA_CMD_HEADER *cmd_hdr = clb;
//     cmd_hdr->prdtl = prdtl;
//     cmd_hdr->pmp = 0;  // if communicate to a directly attached device, set 0;
//     cmd_hdr->rsv0 = 0;
//     cmd_hdr->c = 1;    // clear CI and BSY after transmitting FIS and receiving R_OK
//     cmd_hdr->b = 0;    // if set, enter test mode
//     cmd_hdr->r = 0;    // if set, reset control sequence
//     cmd_hdr->p = 0;    // if set and prdtl is non-zero, prefetch data
//     cmd_hdr->w = write; // 1:H2D, 0:D2H
//     cmd_hdr->a = atapi; // is ATAPI command?
//     cmd_hdr->cfl = cfl; // command fis length
//     cmd_hdr->prdbc = 0; // This is initialized 0 by software (AHCI 5.4.1)
//     cmd_hdr->ctba = (uint32_t)ctba & 0xffffffc0; // must be aligned 128 byte
//     cmd_hdr->ctbau = (uint32_t)(ctba >> 32);
//     rsv1[0] = 0;
//     rsv1[1] = 0;
//     rsv1[2] = 0;
//     rsv1[3] = 0;
// }
// 
// static inline void set_pxsact(HBA_PORT *port, uint32_t slot)
// {
//     // When only PxCMD.ST = 1, software set PxSACT
//     if ((port->cmd & 1) && !(port->sact & slot)) {
//         port->sact |= slot;
//     }
// }
// 
// /* This procedure is only used when PxFB and PxFBU is set */
// static inline void set_pxcmd_st(HBA_PORT *port)
// {
//     port->cmd |= 0x10;  // PxCMD.FRE is set to 1 prior to PxCMD.ST is set to 1
//     while (1) {
//         if (port->tfd & 0x88 == 0) {    // PxTFD.STS.BSY == PxTFD>STS.DRQ == 0
//             port->cmd |= 0x1;   // PxCMD.ST = 1
//             break;
//         }
//     }
// }
// 
// static inline void set_pxci(HBA_PORT *port, uint32_t slot)
// {
//     set_pxcmd_st(port); // set PxCMD.ST to 1
//     // When only PxCMD.ST = 1, software can set PxCI
//     if ((port->cmd & 1) && !(port->ci & slot)) {
//         port->ci |= slot;
//     }
// }
// 
// static uint32_t get_pxci(HBA_PORT *port)
// {
//     return port->ci;
// }
// 
// static uint32_t get_pxis(HBA_PORT *port)
// {
//     return port->is;
// }
// 
// static inline void clear_pxis(HBA_PORT *port)
// {
//     port->is = 0xfd8000af;  // Interrupt Status all reset
// }
// 
// static uint32_t get_ghc_is(void)
// {
//     return ((HBA_MEM_REG *)abar)->is;
// }
// 
// static inline void clear_ghc_is(uint32_t clear_is)
// {
//     ((HBA_MEM_REG *)abar)->is &= (~clear_is);
// }
// 
// #define NCOUNT 512  // 1 count = 512 byte
// static inline void build_command(HBA_PORT *port, CMD_PARAMS *cmd_param)
// {
//     // build command table
//     build_regh2dfis(cmd_param->ctba, cmd_param->cmd_type, cmd_param->lba, cmd_param->count);
//     uint32_t len = cmd_param->count * NCOUNT;
//     build_prdt(cmd_param->ctba, cmd_param->dba, len);
//     // build command header
//     HBA_CMD_HEADER *clb = (HBA_CMD_HEADER *)(uint64_t *)((port->clbu << 32) & port->clb);
//     uint16_t prdtl = len * FOUR_MB;
//     build_command_header(clb, prdtl, write, atapi, cfl, ctba);
//     // set port registers
//     //set_pxsact(port, slot);   // I wouldn't support native queue command
//     uint32_t slot = 0;
//     if (find_cmdslot(port)) {
//         slot = 1 << find_cmdslot(port);
//     } else {
//         puts_serial("Cannot find free command slot.\n");
//         return;
//     }
//     set_pxci(port, slot);
// }
// 
// static inline void process_completed_command(HBA_PORT *port, uint32_t port_bit, uint32_t slot_bit)
// {
//     /* determine and clear the cause of the D2H Register FIS Interrupt */
//     uint32_t pxis = get_pxis(port);
//     if (pxis & 1) { // D2H FIS Register has been received with PRDT.I set 1
//         clear_pxis(port);
//     }
//     /* clear GHC.IS[port_bit] */
//     clear_ghc_is(port_bit);
// 
//     if (get_pxci(port) & slot_bit) {
//         puts_serial("Command is not complete yet.\n");
//     }
// }
// 
// int sata_read(uint64_t sata_lba, uint32_t len)
// {
//     // allocate buffer
//     uint64_t *buf = (uint64_t *)minmalloc((uint64_t)len);
// 
//     // find SATA port
//     struct port_implemented *p_impl;
//     probe_port(p_impl);
//     CMD_PARAMS params;
//     params.cmd_type = READ_DMA_EXT;
//     params.ctba = (uint64_t *);
// 
// }
