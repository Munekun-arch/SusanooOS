#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

// シンプルな RGB マクロ関数
static inline
UINT32 Rgb(UINT8 r, UINT8 g, UINT8 b) {
    return (r << 16) | (g << 8) | b;
}

// 描画関数のプロトタイプ
VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color);
VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color);
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X1, INTN Y1, INTN X2, INTN Y2, UINT32 Color);
VOID DrawTextXY(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, CHAR16 *Text, UINT32 Color);

#endif

