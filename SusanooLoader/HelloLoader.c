#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>   // ← これが必要！

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
  EFI_STATUS Status;

  // GOP を探す
  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
  if (EFI_ERROR(Status)) {
    Print(L"GOP not found!\n");
    return Status;
  }

  // 真っ赤に塗りつぶす
  UINT32 Red = 0x00FF0000;
  UINTN Width  = Gop->Mode->Info->HorizontalResolution;
  UINTN Height = Gop->Mode->Info->VerticalResolution;
  UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;

  for (UINTN y = 0; y < Height; y++) {
    for (UINTN x = 0; x < Width; x++) {
      FrameBuffer[y * Gop->Mode->Info->PixelsPerScanLine + x] = Red;
    }
  }

  Print(L"Hello SusanooOS Loader!\n");
  Print(L"Press any key to exit...\n");

  // キー入力待ち
  EFI_INPUT_KEY Key;
  while (1) {
    Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    if (!EFI_ERROR(Status)) {
      break;
    }
  }

  return EFI_SUCCESS;
}

