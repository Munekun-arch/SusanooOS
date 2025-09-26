#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

  // GOP プロトコルを取得
  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
  if (EFI_ERROR(Status)) {
    Print(L"Unable to locate GOP\n");
    return Status;
  }

  UINTN Width  = Gop->Mode->Info->HorizontalResolution;
  UINTN Height = Gop->Mode->Info->VerticalResolution;
  UINTN PixelsPerScanLine = Gop->Mode->Info->PixelsPerScanLine;

  UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;

  // 背景を真っ赤に塗る
  UINT32 Red = 0x00FF0000;  // BGRA (赤)
  for (UINTN y = 0; y < Height; y++) {
    for (UINTN x = 0; x < Width; x++) {
      FrameBuffer[y * PixelsPerScanLine + x] = Red;
    }
  }

  // 中央に白い四角を描画
  UINTN BoxSize = 200;
  UINTN BoxX0 = (Width - BoxSize) / 2;
  UINTN BoxY0 = (Height - BoxSize) / 2;
  UINT32 White = 0x00FFFFFF; // 白
  for (UINTN y = BoxY0; y < BoxY0 + BoxSize; y++) {
    for (UINTN x = BoxX0; x < BoxX0 + BoxSize; x++) {
      FrameBuffer[y * PixelsPerScanLine + x] = White;
    }
  }

  // 文字を表示
  Print(L"Hello SusanooOS Loader!\n");
  Print(L"Press any key to exit...\n");

  // キー入力待ち
  EFI_INPUT_KEY Key;
  SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) {
    gBS->Stall(100000); // 0.1秒待機
  }

  return EFI_SUCCESS;
}

