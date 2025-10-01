#pragma once
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

static inline
UINT32 Rgb(UINT8 r, UINT8 g, UINT8 b) {
    return ((UINT32)r << 16) | ((UINT32)g << 8) | (UINT32)b;
}

VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 color);
VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
              INTN x, INTN y, INTN w, INTN h, UINT32 color);
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
              INTN x0, INTN y0, INTN x1, INTN y1, UINT32 color);
VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN x, INTN y, UINT32 color);              


