#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include "Graphics.h"
#include "BmpLoader.h"
#include "Text.h"
#include "Window.h"

void EventLoop(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop) {
    EFI_INPUT_KEY Key;
    EFI_STATUS Status;
    UINTN EventIndex;

    Print(L"Event loop started. Press ESC to exit.\n");

    while (TRUE) {
        // 1. キー入力イベント待機
        gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
        Status = gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
        if (EFI_ERROR(Status)) continue;

        // 2. キーの種類で分岐
        if (Key.ScanCode == SCAN_ESC) {
            Print(L"ESC pressed. Exiting event loop.\n");
            break;
        } else if (Key.UnicodeChar != 0) {
            Print(L"Key pressed: %c\n", Key.UnicodeChar);
        }
    }
}

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
    DrawString(Gop, 10, 10, L"Susanoo OS UI Test", Rgb(255, 255, 255), Rgb(0, 0, 180), TRUE);
    
	// ⑦ メニュー項目の描画（描画のみ）
	CONST CHAR16* MenuItems[] = { L"File", L"Settings", L"Exit" };
	INTN MenuCount = sizeof(MenuItems) / sizeof(MenuItems[0]);
	INTN menuX = 20;
	INTN menuY = titleHeight + 5;

	for (INTN i = 0; i < MenuCount; i++) {
    	DrawString(Gop, menuX, menuY, MenuItems[i], Rgb(255,255,255), Rgb(0,0,128), TRUE);
    	menuX += 120;  // 項目の間隔
	}


    // ④ FS プロトコル取得
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

    // ⑥ 下部にテキスト描画（黄文字）
    INTN textY = baseY + 100;
    DrawString(Gop, 40, textY, L"Hello, Susanoo OS!", Rgb(255, 255, 0), Rgb(0, 0, 0), TRUE);
    
	// ウィンドウ管理テスト
	WINDOW win1 = { 50,  80, 200, 120, L"Main Window",   TRUE };
	WINDOW win2 = { 300, 80, 200, 120, L"Second Window", FALSE };

	DrawWindow(Gop, &win1);
	DrawWindow(Gop, &win2);
	

    
    
	// ⑤ 画面描画終了後
	EventLoop(Gop);



    return EFI_SUCCESS;
}

