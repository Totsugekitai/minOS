#include "MinLoader.h"

EFI_STATUS
EFIAPI
Uefi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st)
{
        struct bootinfo_t bootinfo;
        EFI_STATUS status;
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL; // 画面描画に必要

        // GraphicsOutputProtocolを取得
        EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
        status = gBS->LocateProtocol(&gop_guid, NULL, (void**)&gop);

        // カーネルに渡す引数を設定
        bootinfo.vinfo.fb = (unsigned long long)gop->Mode->FrameBufferBase;
        bootinfo.vinfo.fb_size = (unsigned long long)gop->Mode->FrameBufferSize;

        // // ExitBootService()の処理を開始
        // memory mapを取得せなあかんらしいので取得
        UINTN mmapsize = MEM_DESC_SIZE;
        // GetMemoryMapで返ってくる値を格納する変数たち
        UINTN mapkey, descriptorsize;
        UINT32 descriptorversion;
        do {
                // メモリマップ取得
                status = gBS->GetMemoryMap(&mmapsize,
                        (EFI_MEMORY_DESCRIPTOR *)mem_desc, &mapkey,
                        &descriptorsize, &descriptorversion);
                status = gBS->ExitBootServices(image, mapkey);
        } while (EFI_ERROR(status));

        // カーネルに渡す情報をレジスタに格納
        // スタックポインタを設定しカーネルへジャンプ

        start_kernel(&bootinfo);

        return EFI_SUCCESS;
}
