#include "Graphics.h"
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>

VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINT32 Color) {
    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    FrameBuffer[Y * Pitch + X] = Color;
}

VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color) {
    for (INTN dy = 0; dy < H; dy++) {
        for (INTN dx = 0; dx < W; dx++) {
            PutPixel(Gop, X + dx, Y + dy, Color);
        }
    }
}

VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X0, INTN Y0, INTN X1, INTN Y1, UINT32 Color) {
    INTN dx = (X1 > X0) ? (X1 - X0) : (X0 - X1);
    INTN sx = (X0 < X1) ? 1 : -1;
    INTN dy = (Y1 > Y0) ? (Y0 - Y1) : (Y1 - Y0);
    INTN sy = (Y0 < Y1) ? 1 : -1;
    INTN err = dx + dy;

    while (1) {
        PutPixel(Gop, X0, Y0, Color);
        if (X0 == X1 && Y0 == Y1) break;
        INTN e2 = 2 * err;
        if (e2 >= dy) { err += dy; X0 += sx; }
        if (e2 <= dx) { err += dx; Y0 += sy; }
    }
}

VOID DrawText(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *String, INTN X, INTN Y) {
    // とりあえず座標は無視して Print を呼ぶだけ
    Print(L"%s\n", String);
}

