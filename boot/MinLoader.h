#include <stdint.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <X64/ProcessorBind.h>
#include <Guid/FileInfo.h>
#include <Guid/Acpi.h>
#include <Uefi/UefiMultiPhase.h>
#include <Uefi/UefiSpec.h>

#define MEM_DESC_SIZE       0xffff
#define KERNEL_FILE_NAME    L"kernel.bin"
#define BUF_16KB            32768
#define BUF_256B            256
#define KERNEL_START_QEMU   0x00110000
#define BIN_HEADER_SIZE     16

struct video_info_t {
    unsigned long *fb;
    unsigned long fb_size;
    unsigned int x_axis;
    unsigned int y_axis;
    unsigned int ppsl;
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

struct bootinfo_t {
    struct video_info_t vinfo;
    struct RSDP *rsdp;
};

struct header {
    void *bss_start;
    unsigned long long bss_size;
};

extern void kernel_jump(struct bootinfo_t *, uint64_t *);

