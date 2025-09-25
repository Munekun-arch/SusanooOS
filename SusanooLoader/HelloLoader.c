#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
efi_main (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    Print(L"Hello, Susanoo Loader!\n");
    return EFI_SUCCESS;
}

