// Window.c
#include "Window.h"
#include "Graphics.h"
#include "Text.h"

VOID DrawWindow(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, WINDOW *Win) {
    // 外枠
    DrawRect(Gop, Win->X, Win->Y, Win->Width, Win->Height, Rgb(200, 200, 200));

    // タイトルバー
    INTN titleHeight = 20;
    UINT32 titleColor = Win->Active ? Rgb(0, 0, 180) : Rgb(100, 100, 100);
    DrawRect(Gop, Win->X, Win->Y, Win->Width, titleHeight, titleColor);

    // タイトル文字
    DrawString(Gop, Win->X + 5, Win->Y + 4, Win->Title, Rgb(255, 255, 255), titleColor, TRUE);

    // 内容領域（背景）
    DrawRect(Gop, Win->X, Win->Y + titleHeight, Win->Width, Win->Height - titleHeight, Rgb(50, 50, 50));
}

