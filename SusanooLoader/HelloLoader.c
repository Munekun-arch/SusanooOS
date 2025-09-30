#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/FileInfo.h>

#include "Graphics.h"
#include "BmpLoader.h"

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs;
    EFI_FILE_PROTOCOL *Root;

    // LoadedImage を取得
    Status = gBS->HandleProtocol(
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&LoadedImage
    );
    if (EFI_ERROR(Status)) {
        Print(L"LoadedImage not found!\n");
        return Status;
    }

    // SimpleFS を取得
    Status = gBS->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&SimpleFs
    );
    if (EFI_ERROR(Status)) {
        Print(L"SimpleFS not found!\n");
        return Status;
    }

    // ルートディレクトリを開く
    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"OpenVolume failed!\n");
        return Status;
    }

    // GOP 取得
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        return Status;
    }

    // 背景クリア（赤）
    ClearScreen(Gop, 0x000000FF);

    // 基本図形
    DrawRect(Gop, 50, 50, 100, 100, 0x00FF00);     // 緑の矩形
    DrawLine(Gop, 200, 200, 400, 200, 0xFFFFFF);   // 白い水平ライン
    DrawLine(Gop, 200, 200, 200, 400, 0xFFFFFF);   // 白い垂直ライン

    // BMP を複数描画
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\red32.bmp",   100, 300);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\green32.bmp", 150, 300);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\blue32.bmp",  200, 300);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\test.bmp",    250, 300);

    // テキスト出力
    DrawText(0, 0, L"Susanoo OS Graphics Test");

    // 終了待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

