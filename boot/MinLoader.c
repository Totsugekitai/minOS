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
    UINT64 file_size = file_info->FileSize;

    // カーネルファイルをメモリに読み込む
    // ここではカーネルのファイルサイズを16KB以下とする
    unsigned long long *kernel_program = NULL;
    unsigned long long *start_address =
        (unsigned long long *)KERNEL_START_QEMU;
    unsigned long long i;
    buf_size = (UINTN)BUF_16KB;
    do {
        status = kernel_file->Read(kernel_file,
                &buf_size, kernel_program);
    } while(EFI_ERROR(status));
    // bssセクションをゼロクリア
    struct header *head = (struct header *)0x00000000;
    head->bss_address = 0;
    head->bss_size = 0;
    file_size -= sizeof(head);
    // bodyをメモリに書き込む
    for (i = 0; i * 8 < file_size; i++) {
        start_address[i] = kernel_program[i];
    }

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
    kernel_jump(&bootinfo, start_address);

    return EFI_SUCCESS;
}
