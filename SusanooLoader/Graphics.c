#include "Graphics.h"
#include <Library/BaseMemoryLib.h>

VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 color) {
    UINT32* fb = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN pitch = Gop->Mode->Info->PixelsPerScanLine;
    UINTN width = Gop->Mode->Info->HorizontalResolution;
    UINTN height = Gop->Mode->Info->VerticalResolution;

    for (UINTN y = 0; y < height; y++) {
        for (UINTN x = 0; x < width; x++) {
            fb[y * pitch + x] = color;
        }
    }
}

VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
              INTN x, INTN y, INTN w, INTN h, UINT32 color) {
    UINT32* fb = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (INTN j = 0; j < h; j++) {
        for (INTN i = 0; i < w; i++) {
            fb[(y + j) * pitch + (x + i)] = color;
        }
    }
}

VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
              INTN x0, INTN y0, INTN x1, INTN y1, UINT32 color) {
    UINT32* fb = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN pitch = Gop->Mode->Info->PixelsPerScanLine;

    INTN dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    INTN sx = (x0 < x1) ? 1 : -1;
    INTN dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    INTN sy = (y0 < y1) ? 1 : -1;
    INTN err = ((dx > dy) ? dx : -dy) / 2;

    while (TRUE) {
        fb[y0 * pitch + x0] = color;
        if (x0 == x1 && y0 == y1) break;
        INTN e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy)  { err += dx; y0 += sy; }
    }
}

VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN x, INTN y, UINT32 color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    FrameBuffer[y * Pitch + x] = color;
}

//VOID DrawChar(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
//              INTN x, INTN y,
//              CHAR16 ch,
//              UINT32 fg, UINT32 bg,
//              BOOLEAN transparent) {
//    UINT32* fb = (UINT32*)Gop->Mode->FrameBufferBase;
//    UINTN pitch = Gop->Mode->Info->PixelsPerScanLine;

    // 仮: 8x8 の四角を「文字」として描画
//    for (INTN j = 0; j < 8; j++) {
//        for (INTN i = 0; i < 8; i++) {
//            if (transparent) {
//                if (i == 0 || j == 0 || i == 7 || j == 7) {
//                    fb[(y + j) * pitch + (x + i)] = fg;
//                }
//            } else {
//                fb[(y + j) * pitch + (x + i)] = (i == 0 || j == 0 || i == 7 || j == 7) ? fg : bg;
//            }
//        }
//    }
//}

//VOID DrawString(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
//                INTN x, INTN y,
//                const CHAR16 *Str,
//                UINT32 Fg, UINT32 Bg,
//                BOOLEAN Transparent)
//{
//    while (*Str) {   // ← str → Str に修正
//        if (*Str == L'\n') {
//            y += 16;   // フォント高さ
//            x = 0;
//        } else {
//            DrawChar(Gop, x, y, *Str, Fg, Bg, Transparent);
//            x += 8;    // フォント幅
//        }
//        Str++;
//    }
//}


