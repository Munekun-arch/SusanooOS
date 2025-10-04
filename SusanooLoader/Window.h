// Window.h
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

typedef struct {
    INTN X;
    INTN Y;
    INTN Width;
    INTN Height;
    CHAR16 Title[64];
    BOOLEAN Active;
} WINDOW;

// ウィンドウ描画
VOID DrawWindow(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, WINDOW *Win);

#endif

