#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINT8 R, UINT8 G, UINT8 B) {
    if (X >= Gop->Mode->Info->HorizontalResolution || Y >= Gop->Mode->Info->VerticalResolution) {
        return;
    }
    UINT32 *Pixel = (UINT32 *)((UINT8 *)Gop->Mode->FrameBufferBase +
        Y * Gop->Mode->Info->PixelsPerScanLine * 4 +
        X * 4);
    *Pixel = (B << 16) | (G << 8) | R;
}

VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINTN W, UINTN H,
              UINT8 R, UINT8 G, UINT8 B) {
    for (UINTN j = 0; j < H; j++) {
        for (UINTN i = 0; i < W; i++) {
            PutPixel(Gop, X + i, Y + j, R, G, B);
        }
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

    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found\n");
        return Status;
    }

    // 背景黒
    for (UINTN y = 0; y < Gop->Mode->Info->VerticalResolution; y++) {
        for (UINTN x = 0; x < Gop->Mode->Info->HorizontalResolution; x++) {
            PutPixel(Gop, x, y, 0, 0, 0);
        }
    }

    // カラフル矩形を描画
    for (UINTN n = 0; n < 10; n++) {
        UINTN X = (n * 50) % Gop->Mode->Info->HorizontalResolution;
        UINTN Y = (n * 40) % Gop->Mode->Info->VerticalResolution;
        UINTN W = 80;
        UINTN H = 60;
        UINT8 R = (n * 40) % 256;
        UINT8 Gc = (n * 80) % 256;
        UINT8 B = (n * 120) % 256;
        DrawRect(Gop, X, Y, W, H, R, Gc, B);
    }

    Print(L"Colorful Rectangles! Press any key to exit.\n");

    // === ここでキー待ちループ ===
    EFI_INPUT_KEY Key;
    while (1) {
        Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
        if (!EFI_ERROR(Status)) {
            break; // 何か押されたら終了
        }
    }

    return EFI_SUCCESS;
}

