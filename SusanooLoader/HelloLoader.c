#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
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

    Status = gBS->HandleProtocol(
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&LoadedImage
    );
    if (EFI_ERROR(Status)) return Status;

    Status = gBS->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&SimpleFs
    );
    if (EFI_ERROR(Status)) return Status;

    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) return Status;

    // --- 最小実験 ---
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\test.bmp", 100, 100);


    // キー入力待ち
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    Print(L"Press any key to exit...\n");
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

