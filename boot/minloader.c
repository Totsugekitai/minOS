#include <efi.h>
#include <efilib.h>

#include "minloader.h"

#define CONF_FILE       L"boot_qemu.conf"

EFI_SYSTEM_TABLE *gST;
EFI_BOOT_SERVICES *gBS;
EFI_RUNTIME_SERVICES *gRT;

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st)
{
        bootinfo_t bootinfo;
        EFI_STATUS status;
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL; // 画面描画に必要
        EFI_MEMORY_DESCRIPTOR *mmap = NULL; // メモリマップに使う
        EFI_FILE_PROTOCOL *root;

        // UEFI環境を初期化
        gST = st;
        gBS = st->BootService;
        gRT = st->RuntimeService;

        // ファイルシステムからファイルを読み込む準備をする
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
        status = SFSP->OpenVolume(SFSP, (void**)&root);

        // boot_qemu.confを読み込む
        EFI_FILE_PROTOCOL *conf_file;
        status = root->Open(root, (void**)&conf_file,
                CONF_FILE, EFI_FILE_MODE_READ, 0);

        // TODO: boot_qemu.confをパース
        // TODO: カーネルをboot_qemu.confのアドレスにロード
        // TODO: ヘッダの情報に従いbssセクションをゼロクリア

        // GraphicsOutputProtocolを取得
        status = gBS->LocateProtocol(
                        &EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID,
                        NULL,
                        (void**)&gop
                );
        if (!gop) {
                goto errexit;
        }

        // ExitBootService()の処理を開始
        // memory mapを取得せなあかんらしいので取得
        UINTN mmapsize = 0; // まだメモリマップ取得していないので0にしておく
        // GetMemoryMapで返ってくる値を格納する変数たち
        UINTN mapkey, descriptorsize;
        UNIT32 descriptorversion;
        do {
                // メモリマップ取得
                status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                        &descriptorsize, &descriptorversion);
                // メモリが十分になるまで繰り返しメモリを確保
                while (status == EFI_BUFFER_TOO_SMALL) {
                        if (mmap) {
                                gBS->FreePool(mmap);
                        }
                        // EfiLoaderDataはEFI_MEMORY_TYPEで定義
                        status = gBS->AllocatePool(EfiLoaderData,
                                mmapsize, (void**)mmap);
                        status = gBS->GetMemoryMap(&mmapsize, mmap, &mapkey,
                                &descriptorsize, &descriptorversion);
                }
                // BootServicesから抜ける
                status = gBS->ExitBootServices(image, mapkey);
        } while (EFI_ERROR(status));

        // カーネルを起動
        // bootinfoに必要な情報を詰める
        bootinfo.vinfo->fb = (void*)gop->Mode->FrameBufferBase;
        bootinfo.vinfo->fbsize = gop->Mode->FrameBufferSize;
        bootinfo.vinfo->x_axis = gop->Mode->Info->HorizonalResolution;
        bootinfo.vinfo->y_axis = gop->Mode->Info->VerticalResolution;

        // TODO: カーネルに渡す情報をレジスタに格納
        // TODO: スタックポインタを設定しカーネルへジャンプ

        start_kernel(&bootinfo);

errexit:
        return EFI_LOAD_ERROR;
}
