#include "MinLoader.h"

EFI_STATUS
EFIAPI
Uefi_Main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st)
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
    bootinfo.vinfo.fb = (unsigned long long)gop->Mode->FrameBufferBase;
    bootinfo.vinfo.fb_size = (unsigned long long)gop->Mode->FrameBufferSize;

    // SympleFileSystemProtocolを取得
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp = NULL;
    do {
    status = gBS->LocateProtocol(&sfsp_guid, NULL, (void **)&sfsp);
    } while(EFI_ERROR(status));

    // rootディレクトリを開く
    UINT64 sfsp_rev = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION;
    EFI_FILE_PROTOCOL *root = NULL;
    do {
        sfsp->OpenVolume(sfsp, &root);
    } while(EFI_ERROR(status));

    // カーネルのファイルを開く
    EFI_FILE_PROTOCOL *kernel_file = NULL;
    UINT64 fs_rev = EFI_FILE_PROTOCOL_LATEST_REVISION;
    do {
        status = root->Open(root, &kernel_file,
                KERNEL_FILE_NAME,EFI_FILE_READ, 0);
    } while(EFI_ERROR(status));

    // カーネルのファイルサイズを取得
    EFI_FILE_INFO *file_info = NULL;
    EFI_GUID fi_guid = EFI_FILE_INFO_ID;
    do {
        status = kernel_file->GetInfo(kernel_file,
                &fi_guid, BUF_256B, file_info);
    } while(EFI_ERROR(status));
    UINT64 file_size = file_info->FileSize;

    // TODO: カーネルファイルをメモリに読み込む


    // ExitBootService()の処理を開始
    // メモリマップを取得
    UINTN mmapsize = MEM_DESC_SIZE;
    // GetMemoryMapで返ってくる値を格納する変数たち
    UINTN mapkey, descriptorsize;
    UINT32 descriptorversion;
    do {
        // メモリマップ取得
        gBS->GetMemoryMap(&mmapsize, (EFI_MEMORY_DESCRIPTOR *)mem_desc,
                        &mapkey, &descriptorsize, &descriptorversion);
        status = gBS->ExitBootServices(image, mapkey);
    } while (EFI_ERROR(status));

    // カーネルに渡す情報をレジスタに格納
    // スタックポインタを設定しカーネルへジャンプ

    start_kernel(&bootinfo);

    return EFI_SUCCESS;
}
