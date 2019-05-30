#include "MinLoader.h"

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

    // カーネルに渡す引数を設定
    bootinfo.vinfo.fb = (unsigned long *)gop->Mode->FrameBufferBase;
    bootinfo.vinfo.fb_size = (unsigned long)gop->Mode->FrameBufferSize;
    bootinfo.vinfo.x_axis = (unsigned int)gop->Mode->Info->HorizontalResolution;
    bootinfo.vinfo.y_axis = (unsigned int)gop->Mode->Info->VerticalResolution;
    bootinfo.vinfo.ppsl = (unsigned int)gop->Mode->Info->PixelsPerScanLine;

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
    // ここではカーネルのファイルサイズを16KB以下とする
    unsigned long long *kernel_program = NULL;
    unsigned long long *start_address =
        (unsigned long long *)KERNEL_START_QEMU;
    buf_size = (UINTN)BUF_16KB;
    do {
        status = kernel_file->Read(kernel_file,
                &buf_size, kernel_program);
    } while(EFI_ERROR(status));

    // bssセクションをゼロクリア
//    struct header *head = NULL;
//    UINTN head_size = sizeof(head);
//    kernel_file->Read(kernel_file, (UINTN *)head, &head_size);
//    gBS->SetMem(head->bss_address, head->bss_size, 0);
//    file_size -= sizeof(head);
    // bodyをメモリに書き込む
//    gBS->CopyMem(start_address, kernel_program, file_size);
    unsigned long long i;
    for (i = 0; i * 8 < file_size; i++) {
        start_address[i] = kernel_program[i];
    }

    UINTN mmapsize = 0, mapkey, descsize;
    UINT32 descver;
    EFI_MEMORY_DESCRIPTOR *mmap = NULL;
    // ExitBootService()の処理を開始
    do {
        status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                                        &descsize, &descver);
        Print(L"GetMemoryMap.     status: %d, mapkey: %d, mmapsize: %d\n",
            status, mapkey, mmapsize);
        while (status == EFI_BUFFER_TOO_SMALL) {
            if (mmap) {
                gBS->FreePool(mmap);
            }
            // メモリマップの領域を確保
            status = gBS->AllocatePool(EfiLoaderData, mmapsize, (void **)&mmap);
            Print(L"AllocatePool.     status: %d, mmap:   %p, mmapsize: %d\n",
                    status, mmap, mmapsize);
            // メモリマップを取得
            status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                                        &descsize, &descver);
            Print(L"GetMemoryMap.     status: %d, mapkey: %d, mmapsize: %d\n",
                status, mapkey, mmapsize);
        }
        // ExitBootServices
        status = gBS->ExitBootServices(ImageHandle, mapkey);
        Print(L"ExitBootServices. status: %d, mapkey: %d\n", status, mapkey);
    } while (EFI_ERROR(status));

    // カーネルに渡す情報をレジスタに格納
    // スタックポインタを設定しカーネルへジャンプ
    kernel_jump(&bootinfo, start_address);

    return EFI_SUCCESS;
}
