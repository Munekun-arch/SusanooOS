#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

//
// ピクセル描画
//
VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINT32 Color) {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Base =
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Pixel = Base + Y * Pitch + X;
    Pixel->Red   = (Color >> 16) & 0xFF;
    Pixel->Green = (Color >> 8) & 0xFF;
    Pixel->Blue  = (Color) & 0xFF;
}

//
// 線描画（ブレゼンハム法）
//
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN x0, INTN y0, INTN x1, INTN y1, UINT32 Color) {
    INTN dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    INTN sx = (x0 < x1) ? 1 : -1;
    INTN dy = (y1 > y0) ? (y0 - y1) : (y1 - y0);
    INTN sy = (y0 < y1) ? 1 : -1;
    INTN err = dx + dy;
    while (TRUE) {
        PutPixel(Gop, x0, y0, Color);
        if (x0 == x1 && y0 == y1) break;
        INTN e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

    // GOP を取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate GOP\n");
        return Status;
    }

    // 背景を黒で塗りつぶす
    for (UINTN y = 0; y < Gop->Mode->Info->VerticalResolution; y++) {
        for (UINTN x = 0; x < Gop->Mode->Info->HorizontalResolution; x++) {
            PutPixel(Gop, x, y, 0x000000); // 黒
        }
    }

    // テストで線を描く
    DrawLine(Gop, 100, 100, 400, 100, 0xFF0000); // 赤 横線
    DrawLine(Gop, 100, 100, 100, 300, 0x00FF00); // 緑 縦線
    DrawLine(Gop, 100, 100, 400, 300, 0x0000FF); // 青 斜め線

	Print(L"線描画テスト 完了。キーを押すと終了。\n");
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);

	EFI_INPUT_KEY Key;
	while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) == EFI_NOT_READY) {
		gBS->Stall(100000); // 100ms待機
	}

	return EFI_SUCCESS;

}

