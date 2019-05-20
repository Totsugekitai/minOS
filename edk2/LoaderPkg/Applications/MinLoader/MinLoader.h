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

#include "kernel/kernel.h"

#define MEM_DESC_SIZE       4800

unsigned char mem_desc[MEM_DESC_SIZE];
