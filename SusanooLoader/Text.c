#include "Text.h"
#include "Graphics.h"
#include "Font8x16.h"   // 8x16フォントビットマップを仮定

// 1文字描画
VOID DrawChar(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X, INTN Y,
    CHAR16 Ch,
    UINT32 FgRgb, UINT32 BgRgb,
    BOOLEAN TransparentBg
) {
    const UINT8 *glyph = LookupGlyph(Ch);  // Font8x16から取得

    for (INTN yy = 0; yy < FONT_HEIGHT; yy++) {
        UINT8 row = glyph[yy];
        for (INTN xx = 0; xx < FONT_WIDTH; xx++) {
            if (row & (0x80 >> xx)) {
                PutPixel(Gop, X + xx, Y + yy, FgRgb);
            } else if (!TransparentBg) {
                PutPixel(Gop, X + xx, Y + yy, BgRgb);
            }
        }
    }
}

// 文字列描画
VOID DrawString(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X, INTN Y,
    const CHAR16 *Str,
    UINT32 FgRgb, UINT32 BgRgb,
    BOOLEAN TransparentBg
) {
    INTN cx = X;
    for (UINTN i = 0; Str[i] != 0; i++) {
        if (Str[i] == L'\n') {
            cx = X;
            Y += FONT_HEIGHT;
        } else {
            DrawChar(Gop, cx, Y, Str[i], FgRgb, BgRgb, TransparentBg);
            cx += FONT_WIDTH;
        }
    }
}

