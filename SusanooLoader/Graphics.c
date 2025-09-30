#include "Graphics.h"
#include <Library/UefiLib.h>

VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pixels = Gop->Mode->Info->PixelsPerScanLine * Gop->Mode->Info->VerticalResolution;

    for (UINTN i = 0; i < Pixels; i++) {
        FrameBuffer[i] = Color;
    }
}

VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINTN W, UINTN H, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (UINTN dy = 0; dy < H; dy++) {
        for (UINTN dx = 0; dx < W; dx++) {
            FrameBuffer[(Y + dy) * Pitch + (X + dx)] = Color;
        }
    }
}

VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X1, INTN Y1, INTN X2, INTN Y2, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    if (Y1 == Y2) { // 横線
        for (INTN x = X1; x <= X2; x++) {
            FrameBuffer[Y1 * Pitch + x] = Color;
        }
    } else if (X1 == X2) { // 縦線
        for (INTN y = Y1; y <= Y2; y++) {
            FrameBuffer[y * Pitch + X1] = Color;
        }
    }
}

VOID DrawText(INTN X, INTN Y, CHAR16 *Text) {
    // テキストは Print() に任せる（座標制御は未対応）
    Print(L"%s\n", Text);
}

