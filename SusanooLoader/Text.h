#ifndef _TEXT_H_
#define _TEXT_H_

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

// 1文字描画
VOID DrawChar(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X,
    INTN Y,
    CHAR16 Ch,
    UINT32 FgColor,
    UINT32 BgColor,
    BOOLEAN Transparent
);

// 文字列描画
VOID DrawString(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X,
    INTN Y,
    CONST CHAR16 *Str,
    UINT32 FgColor,
    UINT32 BgColor,
    BOOLEAN Transparent
);

#endif

