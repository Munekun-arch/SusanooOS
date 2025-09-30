#include "Graphics.h"

// A/Bだけのフォント
STATIC CONST UINT8 Font8x16[2][16] = {
    {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00,0,0,0,0,0,0,0,0}, // 'A'
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00,0,0,0,0,0,0,0,0}  // 'B'
};

// 背景クリア
#include "Graphics.h"

VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Width = Gop->Mode->Info->HorizontalResolution;
    UINTN Height = Gop->Mode->Info->VerticalResolution;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (UINTN y = 0; y < Height; y++) {
        for (UINTN x = 0; x < Width; x++) {
            FrameBuffer[y * Pitch + x] = Color;
        }
    }
}


// 矩形
VOID
DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
         INTN X, INTN Y, INTN Width, INTN Height, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (INTN y = 0; y < Height; y++) {
        for (INTN x = 0; x < Width; x++) {
            FrameBuffer[(Y + y) * Pitch + (X + x)] = Color;
        }
    }
}

// ライン（水平/垂直のみ）
VOID
DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
         INTN X1, INTN Y1, INTN X2, INTN Y2, UINT32 Color) {
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    if (Y1 == Y2) {
        for (INTN x = X1; x <= X2; x++) {
            FrameBuffer[Y1 * Pitch + x] = Color;
        }
    } else if (X1 == X2) {
        for (INTN y = Y1; y <= Y2; y++) {
            FrameBuffer[y * Pitch + X1] = Color;
        }
    }
}

// 内部文字描画
STATIC
VOID
DrawChar(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
         INTN X, INTN Y, CHAR16 Ch, UINT32 Color) {
    if (Ch < L'A' || Ch > L'B') return;
    CONST UINT8 *Glyph = Font8x16[Ch - L'A'];
    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (INTN row = 0; row < 16; row++) {
        UINT8 Bits = Glyph[row];
        for (INTN col = 0; col < 8; col++) {
            if (Bits & (1 << (7 - col))) {
                FrameBuffer[(Y + row) * Pitch + (X + col)] = Color;
            }
        }
    }
}

// テキスト
VOID
DrawTextXY(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
           INTN X, INTN Y, CHAR16 *String, UINT32 Color) {
    INTN CursorX = X;
    while (*String) {
        DrawChar(Gop, CursorX, Y, *String, Color);
        CursorX += 8;
        String++;
    }
}

