#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h> // ← これが必要！
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_STATUS Status;

    // GOP 取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate GOP\n");
        return Status;
    }

    // 画面サイズ取得
    UINTN width   = Gop->Mode->Info->HorizontalResolution;
    UINTN height = Gop->Mode->Info->VerticalResolution;

    // Step A: 背景を黒で塗りつぶし
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black = {0x00, 0x00, 0x00, 0x00};
    Gop->Blt(
        Gop,
        &Black,
        EfiBltVideoFill,
        0, 0, 0, 0,
        width,
        height,
        0
    );

  // Step A: 解像度表示
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut = SystemTable->ConOut;
  CHAR16 Buffer[128];
  UnicodeSPrint(Buffer, sizeof(Buffer), L"Step A: Resolution %ux%u\n",
              Gop->Mode->Info->HorizontalResolution,
              Gop->Mode->Info->VerticalResolution);
  ConOut->OutputString(ConOut, Buffer);

  gBS->Stall(2000000); // 2秒停止

  // ===============================
  // Step A: 背景をグラデーションで塗る
  // ===============================
  for (UINTN x = 0; x < width; x++) {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = {
      (UINT8)(x % 256),   //  B成分を変化させる
      0x00,                   // G
      0x00,                   // R
      0x00                    // Reserved
    };
    Gop->Blt(
      Gop,
      &color,
      EfiBltVideoFill,
      0, 0,
      x, 0,
      1, height,
      0
    );
  }

  // ===============================
  // Step B: 中央に赤い矩形を描画
  // ===============================
  UINTN rectWidth = 200;
  UINTN rectHeight = 100;
  UINTN x0 = (width - rectWidth) / 2;
  UINTN y0 = (height - rectHeight) / 2;

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL Red = { 0x00, 0x00, 0xFF, 0x00 };
  Gop->Blt(
    Gop,
    &Red,
    EfiBltVideoFill,
    0, 0,
    x0, y0,
    rectWidth, rectHeight,
    0
  );
  Print(L"Step B: Red rectangle drawn.\n");

  // ===============================
  // Step C: キー入力待ち
  // ===============================
  EFI_INPUT_KEY Key;
  SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
  Print(L"Step C: Press any key to exit...\n");
  SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

  return EFI_SUCCESS;
}
