#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

// 画面クリア
VOID ClearScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 Color);

// 矩形描画
VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINTN W, UINTN H, UINT32 Color);

// ライン描画 (水平/垂直のみ)
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X1, INTN Y1, INTN X2, INTN Y2, UINT32 Color);

// テキスト描画（位置指定）
VOID DrawText(INTN X, INTN Y, CHAR16 *Text);

#endif

