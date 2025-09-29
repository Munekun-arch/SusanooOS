#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include "BmpLoader.h"   // BMP ローダーの宣言

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

    // 複数のBMPを描画するテスト
	LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\red32.bmp",   100, 100);
	LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\green32.bmp", 150, 100);
	LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\blue32.bmp",  200, 100);
	LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\test.bmp",    250, 100);

    // キー入力待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

