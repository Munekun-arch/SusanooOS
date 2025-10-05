// Window.c
#include "Window.h"
#include "Graphics.h"
#include "Text.h"

VOID DrawWindow(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, WINDOW *Win) {
    // 背景
    DrawRect(Gop, Win->X, Win->Y, Win->Width, Win->Height, Win->BgColor);

    // タイトルバー
    INTN titleHeight = 20;
    DrawRect(Gop, Win->X, Win->Y, Win->Width, titleHeight, Win->TitleColor);

    // タイトル文字
    DrawString(Gop, Win->X + 5, Win->Y + 4, Win->Title, Rgb(255,255,255), Win->TitleColor, TRUE);

    // 枠線
    DrawRect(Gop, Win->X, Win->Y, Win->Width, 1, Rgb(255,255,255)); // 上
    DrawRect(Gop, Win->X, Win->Y + Win->Height - 1, Win->Width, 1, Rgb(255,255,255)); // 下
    DrawRect(Gop, Win->X, Win->Y, 1, Win->Height, Rgb(255,255,255)); // 左
    DrawRect(Gop, Win->X + Win->Width - 1, Win->Y, 1, Win->Height, Rgb(255,255,255)); // 右
}

