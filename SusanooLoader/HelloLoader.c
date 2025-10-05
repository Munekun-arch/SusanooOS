#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimplePointer.h>

#include "Graphics.h"
#include "BmpLoader.h"
#include "Text.h"
#include "Window.h"   // ← 追加済み（構造体対応）

// ======================================================
// キー入力イベントループ（ESCで終了）
// ======================================================
void EventLoop(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, EFI_SIMPLE_POINTER_PROTOCOL *Mouse) {
    EFI_INPUT_KEY Key;
    EFI_STATUS Status;
    EFI_SIMPLE_POINTER_STATE MouseState; 
    UINTN EventIndex;

    // カーソル座標初期化
    INTN MouseX = Gop->Mode->Info->HorizontalResolution / 2;
    INTN MouseY = Gop->Mode->Info->VerticalResolution / 2;
    const INTN CursorSize = 8;

    // 初期カーソル描画
    DrawRect(Gop, MouseX, MouseY, CursorSize, CursorSize, Rgb(255,255,255));

    while (TRUE) {
        EFI_EVENT Events[2];
        UINTN EventCount = 1;
        Events[0] = gST->ConIn->WaitForKey;

        if (Mouse) {
            Events[1] = Mouse->WaitForInput;
            EventCount = 2;
        }

        gBS->WaitForEvent(EventCount, Events, &EventIndex);

        if (EventIndex == 0) {
            Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
            if (!EFI_ERROR(Status) && Key.ScanCode == SCAN_ESC) {
                break;
            }
        } else if (Mouse && EventIndex == 1) {
            Status = Mouse->GetState(Mouse, &MouseState);
            if (EFI_ERROR(Status)) continue;

            // 旧カーソルを消す
            DrawRect(Gop, MouseX, MouseY, CursorSize, CursorSize, Rgb(0,0,0));

            // 相対移動を反映
            MouseX += MouseState.RelativeMovementX / 2;
            MouseY -= MouseState.RelativeMovementY / 2;

            // 画面内に収める
            if (MouseX < 0) MouseX = 0;
            if (MouseY < 0) MouseY = 0;
            if (MouseX > (INTN)Gop->Mode->Info->HorizontalResolution - CursorSize)
                MouseX = Gop->Mode->Info->HorizontalResolution - CursorSize;
            if (MouseY > (INTN)Gop->Mode->Info->VerticalResolution - CursorSize)
                MouseY = Gop->Mode->Info->VerticalResolution - CursorSize;

            // 新カーソル描画
            DrawRect(Gop, MouseX, MouseY, CursorSize, CursorSize, Rgb(255,255,255));
        }
    }
}



// ======================================================
// エントリーポイント
// ======================================================
EFI_STATUS
EFIAPI
UefiMain (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_STATUS Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs;
    EFI_FILE_PROTOCOL *Root;

    // ① GOP 取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate GOP: %r\n", Status);
        return Status;
    }

    // ② 画面クリア（黒）
    ClearScreen(Gop, Rgb(0, 0, 0));

    // ③ タイトルバー描画（青地に白文字）
    INTN titleHeight = 40;
    DrawRect(Gop, 0, 0, Gop->Mode->Info->HorizontalResolution, titleHeight, Rgb(0, 0, 180));
      DrawString(Gop, 10, 10, L"Mouse Test", Rgb(255,255,0), Rgb(0,0,0), TRUE);

    // 🖱 マウスプロトコル取得
	EFI_SIMPLE_POINTER_PROTOCOL *Mouse;
	Status = gBS->LocateProtocol(&gEfiSimplePointerProtocolGuid, NULL, (VOID**)&Mouse);
	if (EFI_ERROR(Status)) {
	    Print(L"Mouse protocol not found: %r\n", Status);
	    Mouse = NULL;
	}

    // ④ ファイルシステム取得
    Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to get LoadedImage: %r\n", Status);
        return Status;
    }

    Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFs);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to locate FS: %r\n", Status);
        return Status;
    }

    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"Failed to open volume: %r\n", Status);
        return Status;
    }

    // ⑤ BMP配置座標
    INTN baseY = titleHeight + 20;
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\red32.bmp",   50, baseY);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\green32.bmp", 150, baseY);
    LoadBmpAndDraw(Root, L"\\EFI\\BOOT\\blue32.bmp",  250, baseY);

    // ⑥ テキスト描画（黄文字）
    INTN textY = baseY + 100;
    DrawString(Gop, 40, textY, L"Hello, Susanoo OS!", Rgb(255, 255, 0), Rgb(0, 0, 0), TRUE);

    // ⑦ ウィンドウ描画（構造体対応）
    WINDOW MainWindow = { 100, 300, 300, 200, L"Main Window", Rgb(200,200,255), Rgb(0,0,128) };
    DrawWindow(Gop, &MainWindow);

    WINDOW SecondWindow = { 450, 300, 250, 150, L"Second Window", Rgb(255,255,200), Rgb(128,0,0) };
    DrawWindow(Gop, &SecondWindow);

    // ⑧ イベントループ
    EventLoop(Gop, Mouse);

    return EFI_SUCCESS;
}

