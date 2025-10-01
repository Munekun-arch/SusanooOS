#pragma once
#include <Uefi.h>

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

// ASCII 文字のフォントビットマップ (例: 'A' のみ)
static const UINT8 Font8x16[256][FONT_HEIGHT] = {
    // 'A' = 65
    [65] = {
        0b00011000,
        0b00100100,
        0b01000010,
        0b01000010,
        0b01111110,
        0b01000010,
        0b01000010,
        0b01000010,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    }
};

// 文字コードに対応するグリフを返す
static inline const UINT8* LookupGlyph(CHAR16 ch) {
    if (ch < 256) {
        return Font8x16[ch];
    }
    return Font8x16[0]; // 未定義文字は空白扱い
}

