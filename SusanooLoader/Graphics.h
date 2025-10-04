#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

//
// RGB ユーティリティ
//
static inline UINT32 Rgb(UINT8 r, UINT8 g, UINT8 b) {
    return ((UINT32)r << 16) | ((UINT32)g << 8) | (UINT32)b;
}

//
// 基本描画系
//
VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINT32 Color);
VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color);
VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color);
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X0, INTN Y0, INTN X1, INTN Y1, UINT32 Color);

VOID DrawString(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X,
    INTN Y,
    CONST CHAR16 *Str,
    UINT32 FgColor,
    UINT32 BgColor,
    BOOLEAN Transparent
);
VOID DrawCloseButton(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y);



#endif

