#pragma once
#include <Uefi.h>

#define FONT_WIDTH   8
#define FONT_HEIGHT 16

// 0x20（スペース）〜0x7E（~）まで対応
extern const UINT8 Font8x16[95][16];

// 文字コードからフォントデータへのポインタを取得
static inline const UINT8* LookupGlyph(CHAR16 ch) {
    if (ch < 0x20 || ch > 0x7E) {
        return Font8x16[0];  // 未対応文字はスペース扱い
    }
    return Font8x16[ch - 0x20];
}

