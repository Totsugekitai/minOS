#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <X64/ProcessorBind.h>
#include <Guid/FileInfo.h>
#include <Uefi/UefiMultiPhase.h>
#include <Uefi/UefiSpec.h>

#define MEM_DESC_SIZE       4800
#define KERNEL_FILE_NAME    L"kernel.bin"
#define BUF_16KB            16384
#define BUF_256B            256
#define KERNEL_START_QEMU   0x00110000
#define BIN_HEADER_SIZE     16

struct video_info_t {
    unsigned long long fb;
    unsigned long long fb_size;
};

struct bootinfo_t {
    struct video_info_t vinfo;
};

struct header {
    void *bss_address;
    unsigned long long bss_size;
};

unsigned char mem_desc[MEM_DESC_SIZE];

extern void kernel_jump(struct bootinfo_t *, unsigned long long *);

