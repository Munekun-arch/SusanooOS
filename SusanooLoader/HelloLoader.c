#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
  EFI_STATUS Status;

  // GOPを取得
  Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
  if (EFI_ERROR(Status)) {
    Print(L"GOP not found\n");
    return Status;
  }

  UINT32 Width  = Gop->Mode->Info->HorizontalResolution;
  UINT32 Height = Gop->Mode->Info->VerticalResolution;
  UINTN  Pixels = Width * Height;

  // 1ピクセル = 4バイト (BGRA)
  UINT32 *FrameBuffer;
  FrameBuffer = AllocatePool(Pixels * sizeof(UINT32));
  if (!FrameBuffer) {
    Print(L"Failed to allocate buffer\n");
    return EFI_OUT_OF_RESOURCES;
  }

  // バッファに描画（ここではグラデーション）
  for (UINT32 y = 0; y < Height; y++) {
    for (UINT32 x = 0; x < Width; x++) {
      UINT8 r = (x * 255) / Width;   // 横方向グラデーション
      UINT8 g = (y * 255) / Height;  // 縦方向グラデーション
      UINT8 b = 128;                 // 固定ブルー
      FrameBuffer[y * Width + x] = (b) | (g << 8) | (r << 16);
    }
  }

  // 転送用のBLTバッファに詰め替え
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  BltBuffer = AllocatePool(Pixels * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  if (!BltBuffer) {
    FreePool(FrameBuffer);
    Print(L"Failed to allocate BLT buffer\n");
    return EFI_OUT_OF_RESOURCES;
  }

  for (UINTN i = 0; i < Pixels; i++) {
    UINT32 c = FrameBuffer[i];
    BltBuffer[i].Blue  = (UINT8)(c & 0xFF);
    BltBuffer[i].Green = (UINT8)((c >> 8) & 0xFF);
    BltBuffer[i].Red   = (UINT8)((c >> 16) & 0xFF);
    BltBuffer[i].Reserved = 0;
  }

  // 画面に一気に描画
  Status = Gop->Blt(
    Gop,
    BltBuffer,
    EfiBltBufferToVideo,
    0, 0, 0, 0,
    Width, Height,
    0
  );

   // 表示保持
  Print(L"Press any key to exit...\n");
  EFI_INPUT_KEY Key;
  UINTN Index;

  // イベントが来るまで待つ
  gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);

  // 実際のキー入力を取得
  gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

  FreePool(FrameBuffer);
  FreePool(BltBuffer);
  return EFI_SUCCESS;
}

