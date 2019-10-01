#include "MinLoader.h"

int IsEqualGUID(EFI_GUID *guid1, EFI_GUID *guid2);

EFI_STATUS
EFIAPI
Uefi_Main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *st)
{
    struct bootinfo_t bootinfo;
    EFI_STATUS status;

    // GraphicsOutputProtocolを取得
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
    do {
        status = gBS->LocateProtocol(&gop_guid, NULL, (void **)&gop);
    } while (EFI_ERROR(status));

    // EfiConfigrationTable中のAcpiを取得
    EFI_GUID at_guid = EFI_ACPI_20_TABLE_GUID;
    void *acpi_table;
    for (int i = 0; i < gST->NumberOfTableEntries; i++) {
        EFI_CONFIGURATION_TABLE *config_table = gST->ConfigurationTable + i;
        if (IsEqualGUID(&config_table->VendorGuid, &at_guid)) {
            acpi_table = config_table->VendorTable;
            break;
        }
        acpi_table = NULL;
    }

    // カーネルに渡す引数を設定
    // video_infoを詰める
    bootinfo.vinfo.fb = (uint64_t *)gop->Mode->FrameBufferBase;
    bootinfo.vinfo.fb_size = (uint64_t)gop->Mode->FrameBufferSize;
    bootinfo.vinfo.x_axis = (uint32_t)gop->Mode->Info->HorizontalResolution;
    bootinfo.vinfo.y_axis = (uint32_t)gop->Mode->Info->VerticalResolution;
    bootinfo.vinfo.ppsl = (uint32_t)gop->Mode->Info->PixelsPerScanLine;
    // acpi_infoを詰める
    bootinfo.rsdp = (struct RSDP *)acpi_table;

    // SympleFileSystemProtocolを取得
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp = NULL;
    do {
    status = gBS->LocateProtocol(&sfsp_guid, NULL, (void **)&sfsp);
    } while(EFI_ERROR(status));

    // rootディレクトリを開く
    EFI_FILE_PROTOCOL *root = NULL;
    do {
        sfsp->OpenVolume(sfsp, &root);
    } while(EFI_ERROR(status));

    // カーネルのファイルを開く
    EFI_FILE_PROTOCOL *kernel_file = NULL;
    CHAR16 *file_name;
    UINT64 fr_num = (UINT64)EFI_FILE_READ_ONLY;
    file_name = (CHAR16 *)KERNEL_FILE_NAME;
    do {
        status = root->Open(root, &kernel_file,
                file_name, fr_num, 0);
    } while(EFI_ERROR(status));

    // カーネルのファイルサイズを取得
    EFI_FILE_INFO *file_info = NULL;
    EFI_GUID fi_guid = EFI_FILE_INFO_ID;
    UINTN buf_size = (UINTN)BUF_256B;
    do {
        status = kernel_file->GetInfo(kernel_file,
                &fi_guid, &buf_size, file_info);
    } while(EFI_ERROR(status));
    UINTN file_size = file_info->FileSize;

    // カーネルファイルをメモリに読み込む
    // ここではカーネルのファイルサイズを512KB以下とする
    uint64_t *kernel_program = NULL;
    uint64_t *start_address =
        (uint64_t *)KERNEL_START_QEMU;
    buf_size = (UINTN)BUF_512KB;
    do {
        status = kernel_file->Read(kernel_file,
                &buf_size, kernel_program);
    } while(EFI_ERROR(status));

    // bodyをメモリに書き込む
    gBS->CopyMem(start_address, kernel_program, file_size);

    // メモリマップ取得のための変数
    UINTN mmapsize = 0, mapkey, descsize;
    UINT32 descver;
    EFI_MEMORY_DESCRIPTOR *mmap = NULL;
    // ExitBootService()の処理を開始
    do {
        status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                                        &descsize, &descver);
        while (status == EFI_BUFFER_TOO_SMALL) {
            if (mmap) {
                gBS->FreePool(mmap);
            }
            mmapsize += 0x1000;
            // メモリマップの領域を確保
            status = gBS->AllocatePool(EfiLoaderData, mmapsize, (void **)&mmap);
            // メモリマップを取得
            status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                                        &descsize, &descver);
        }
        // ExitBootServices
        status = gBS->ExitBootServices(ImageHandle, mapkey);
    } while (EFI_ERROR(status));

    // メモリマップやらを取得
    bootinfo.memory_map = mmap;
    bootinfo.mmapsize = mmapsize;
    bootinfo.memdescsize = descsize;

    // カーネルに渡す情報をレジスタに格納
    // スタックポインタを設定しカーネルへジャンプ
    kernel_jump(&bootinfo, start_address);

    return EFI_SUCCESS;
}

// GUIDの比較
int IsEqualGUID(EFI_GUID *guid1, EFI_GUID *guid2)
{
    uint64_t *s = (uint64_t *)guid1;
    uint64_t *t = (uint64_t *)guid2;
    return (s[0] == t[0]) && (s[1] == t[1]);
}

