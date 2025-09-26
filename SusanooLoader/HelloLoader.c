#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_INPUT_KEY Key;

  Print(L"Hello SusanooOS Loader!\n");
  Print(L"Press any key to continue...\n");

  // キー入力があるまで待機
  while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) == EFI_NOT_READY) {
    gBS->Stall(100000); // 100ms 待機
  }

  return EFI_SUCCESS;
}

