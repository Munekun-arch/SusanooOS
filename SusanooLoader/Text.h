#pragma once
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

VOID DrawChar(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X, INTN Y,
    CHAR16 Ch,
    UINT32 FgRgb, UINT32 BgRgb,
    BOOLEAN TransparentBg
);

VOID DrawString(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    INTN X, INTN Y,
    const CHAR16 *Str,
    UINT32 FgRgb, UINT32 BgRgb,
    BOOLEAN TransparentBg
);

