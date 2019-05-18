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

#include "kernel.h"

#define SAFETY_FILE_UNIT        16384 // 16KB

struct config {
        char kernel_start[17];
};

struct header {
        void *bss_start;
        unsigned long long bss_size;
};

unsigned long long get_file_size(EFI_FILE_PROTOCOL *file);
void safety_file_read(EFI_FILE_PROTOCOL *src, void *dst,
                        unsigned long long size);

extern void kernel_jmp(unsigned long long, unsigned long long,
                    unsigned long long, unsigned long long);

#define QEMU_MMAP       0x0000000000110000
#define KERNEL_FILE     L"kernel.bin"
#define FILE_INFO_BUF_SIZE      180
#define MB              1048576 // 1024 * 1024
#define MEM_DESC_SIZE       4800
// EFI_SYSTEM_TABLE *gST;
// EFI_BOOT_SERVICES *gBS;
// EFI_RUNTIME_SERVICES *gRT;

unsigned char mem_desc[MEM_DESC_SIZE];

EFI_STATUS
EFIAPI
Uefi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st)
{
        struct bootinfo_t bootinfo;
        EFI_STATUS status;
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL; // 画面描画に必要
        EFI_MEMORY_DESCRIPTOR *mmap = NULL; // メモリマップに使う
        EFI_FILE_PROTOCOL *root;
        Print(L"57\n");
        // UEFI環境を初期化
        // gST = st;
        // gBS = st->BootServices;
        // gRT = st->RuntimeServices;

        // ファイルシステムからファイルを読み込む準備をする
        EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
        gBS->LocateProtocol(&sfsp_guid, NULL, (void **)&SFSP);
        status = SFSP->OpenVolume(SFSP, &root);
        Print(L"68\n");
        unsigned long long kernel_start = QEMU_MMAP;
        //カーネルファイルを開く
        EFI_FILE_PROTOCOL *kernel_file;
        status = root->Open(
                root, &kernel_file, KERNEL_FILE, EFI_FILE_MODE_READ, 0);
        // カーネルファイルのサイズを取得
        unsigned long long kernel_size = get_file_size(kernel_file);
        Print(L"76\n");
        // ヘッダの情報に従いbssセクションをゼロクリア
        // stack_baseはスタックの底のアドレス(上へ伸びる)
        unsigned long long stack_base = kernel_start + (1 * MB);

        struct header head;
        int head_size = sizeof(head);
        safety_file_read(kernel_file, (void *)&head, head_size);

        kernel_size -= sizeof(head);

        safety_file_read(kernel_file, (void *)kernel_start, kernel_size);
        kernel_file->Close(kernel_file);
        Print(L"89\n");
        // ゼロクリア
        gBS->SetMem(head.bss_start, head.bss_size, 0);

        // GraphicsOutputProtocolを取得
        EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
        status = gBS->LocateProtocol(&gop_guid, NULL, (void**)&gop);




        Print(L"99\n");
        // カーネルに渡す引数を設定
        unsigned long long kernel_arg1 = (unsigned long long)st;
        Print(L"101\n");
        bootinfo.vinfo.fb = (unsigned long long)gop->Mode->FrameBufferBase;
        Print(L"103\n");
        bootinfo.vinfo.fb_size = (unsigned long long)gop->Mode->FrameBufferSize;
        Print(L"105\n");
        unsigned long long kernel_arg2 = (unsigned long long)&bootinfo;
        Print(L"107\n");
        Print(L"bootinfo:{vinfo:{fb: %p, fb_size: %d}}\n",
            bootinfo.vinfo.fb, bootinfo.vinfo.fb_size);
        Print(L"kernel_arg1 address: %lx\n", kernel_arg1);
        Print(L"kernel_arg2 address: %lx\n", kernel_arg2);
        Print(L"stack_base address: %lx\n", stack_base);
        Print(L"kernel_start address: %lx\n", kernel_start);
        // ExitBootService()の処理を開始
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
                // メモリが十分になるまで繰り返しメモリを確保
                while (status == EFI_BUFFER_TOO_SMALL) {
                        if (mmap) {
                                gBS->FreePool(mmap);
                        }
                        // EfiLoaderDataはEFI_MEMORY_TYPEで定義
                        status = gBS->AllocatePool(EfiLoaderData,
                                mmapsize, (void**)mmap);
                        status = gBS->GetMemoryMap(&mmapsize,
                                (EFI_MEMORY_DESCRIPTOR *)mem_desc, &mapkey,
                                &descriptorsize, &descriptorversion);
                }
                // BootServicesから抜ける
                Print(L"131\n");
                status = gBS->ExitBootServices(image, mapkey);
        } while (EFI_ERROR(status));

        // カーネルに渡す情報をレジスタに格納
        // スタックポインタを設定しカーネルへジャンプ

        start_kernel(&bootinfo);

        unsigned long long _sb = stack_base, _ks = kernel_start;
        Print(L"kernel_arg1 address: %lx\n", kernel_arg1);
        Print(L"kernel_arg2 address: %lx\n", kernel_arg2);
        Print(L"stack_base address: %lx\n", _sb);
        Print(L"kernel_start address: %lx\n", _ks);
        kernel_jmp(kernel_arg1, kernel_arg2, _sb, _ks);

        return EFI_SUCCESS;
}

unsigned long long get_file_size(EFI_FILE_PROTOCOL *file)
{
        unsigned long long status, fi_size = FILE_INFO_BUF_SIZE,
                      fi_buf[FILE_INFO_BUF_SIZE];
        EFI_FILE_INFO *fi_ptr;
        EFI_GUID fi_guid = EFI_FILE_INFO_ID;

        status = file->GetInfo(file, &fi_guid, &fi_size, fi_buf);
        fi_ptr = (EFI_FILE_INFO *)fi_buf;
        Print(L"get_file_size status: %lx", status);
        return fi_ptr->FileSize;
}

void safety_file_read(EFI_FILE_PROTOCOL *src, void *dst,
                        unsigned long long size)
{
        unsigned long long status;
        unsigned char *d = dst;
        while (size > SAFETY_FILE_UNIT) {
                unsigned long long unit = SAFETY_FILE_UNIT;
                status = src->Read(src, &unit, (void*)d);
                d += unit;
                size -= unit;
        }
        while (size > 0) {
                unsigned long long tmp_size = size;
                status = src->Read(src, &tmp_size, (void *)d);
                size -= tmp_size;
        }
        Print(L"safety_file_read status: %lx", status);
}
