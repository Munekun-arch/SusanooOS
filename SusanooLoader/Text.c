#include "Text.h"
#include "Graphics.h"
#include "Font8x16.h"

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

VOID DrawChar(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X,
    INTN Y,
    CHAR16 Ch,
    UINT32 FgColor,
    UINT32 BgColor,
    BOOLEAN Transparent
) {
    if (Ch < 32 || Ch > 127) return;
    CONST UINT8 *glyph = Font8x16[Ch - 32];

    for (INTN yy = 0; yy < FONT_HEIGHT; yy++) {
        UINT8 line = glyph[yy];
        for (INTN xx = 0; xx < FONT_WIDTH; xx++) {
            if (line & (1 << xx)) {
                PutPixel(Gop, X + xx, Y + yy, FgColor);
            } else if (!Transparent) {
                PutPixel(Gop, X + xx, Y + yy, BgColor);
            }
        }
    }
}

VOID DrawString(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X,
    INTN Y,
    CONST CHAR16 *Str,
    UINT32 FgColor,
    UINT32 BgColor,
    BOOLEAN Transparent
) {
    INTN cx = X;
    INTN cy = Y;

    for (UINTN i = 0; Str[i] != 0; i++) {
        if (Str[i] == L'\n') {
            cx = X;
            cy += FONT_HEIGHT;
        } else {
            DrawChar(Gop, cx, cy, Str[i], FgColor, BgColor, Transparent);
            cx += FONT_WIDTH;
        }
    }
}

