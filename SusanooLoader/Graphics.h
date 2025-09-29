#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

// ピクセル描画
VOID PutPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, UINT32 Color);

// 矩形描画
VOID DrawRect(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X, INTN Y, INTN W, INTN H, UINT32 Color);

// ライン描画（Bresenham）
VOID DrawLine(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, INTN X0, INTN Y0, INTN X1, INTN Y1, UINT32 Color);

// テキスト描画（簡易版）
VOID DrawText(EFI_SYSTEM_TABLE *SystemTable, CHAR16 *String, INTN X, INTN Y);

#endif

