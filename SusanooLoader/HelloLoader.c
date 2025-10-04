#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>

#include "Graphics.h"
#include "BmpLoader.h"
#include "Text.h"

EFI_STATUS
EFIAPI
UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs;
    EFI_FILE_PROTOCOL *Root;

    // ① GOP 取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate GOP: %r\n", Status);
        return Status;
    }

    // ② 画面クリア（黒）
    ClearScreen(Gop, Rgb(0, 0, 0));

    // ③ タイトルバー描画（青地に白文字）
    INTN titleHeight = 40;
    DrawRect(Gop, 0, 0, Gop->Mode->Info->HorizontalResolution, titleHeight, Rgb(0, 0, 180));
    DrawString(Gop, 10, 10, L"Susanoo OS UI Test", Rgb(255, 255, 255), Rgb(0, 0, 180), TRUE);

    // ④ FS プロトコル取得
    Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get LoadedImage: %r\n", Status);
        return Status;
    }

    Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFs);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate FS: %r\n", Status);
        return Status;
    }

    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open volume: %r\n", Status);
        return Status;
    }

    // ⑤ BMP配置座標
    INTN baseY = titleHeight + 20;
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\red32.bmp",   50, baseY);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\green32.bmp", 150, baseY);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\blue32.bmp",  250, baseY);

    // ⑥ 下部にテキスト描画（黄文字）
    INTN textY = baseY + 100;
    DrawString(Gop, 40, textY, L"Hello, Susanoo OS!", Rgb(255, 255, 0), Rgb(0, 0, 0), TRUE);

    // ⑦ 終了前のキー入力待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

