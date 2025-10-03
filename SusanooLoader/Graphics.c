#include "Graphics.h"

VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINT32 Color) {
    if (X < 0 || Y < 0) return;
    if ((UINTN)X >= Gop->Mode->Info->HorizontalResolution ||
        (UINTN)Y >= Gop->Mode->Info->VerticalResolution) {
        return;
    }

    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    FrameBuffer[Y * Pitch + X] = Color;
}

VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color) {
    UINTN Width  = Gop->Mode->Info->HorizontalResolution;
    UINTN Height = Gop->Mode->Info->VerticalResolution;

    for (UINTN y = 0; y < Height; y++) {
        for (UINTN x = 0; x < Width; x++) {
            PutPixel(Gop, x, y, Color);
        }
    }
}

VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color) {
    for (INTN yy = 0; yy < H; yy++) {
        for (INTN xx = 0; xx < W; xx++) {
            PutPixel(Gop, X + xx, Y + yy, Color);
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
        if (e2 >= dy) {
            err += dy;
            X0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            Y0 += sy;
        }
    }
}

