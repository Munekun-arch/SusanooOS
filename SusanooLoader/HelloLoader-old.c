#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

	// ① GOP 取得
	Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
	if (EFI_ERROR(Status) || !Gop) {
		Print(L"GOP NG: %r\n", Status);
		return Status;
	}

	// ② 画面情報の表示
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info = Gop->Mode->Info;
	Print(L"RES=%ux%u, PF=%u\n",
			info->HorizontalResolution, info->VerticalResolution,
			info->PixelFormat);
	if (info->PixelFormat == PixelBitMask) {
		Print(L"Mask R=%08x G=%08x B=%08x Res=%08x\n",
				info->PixelInformation.RedMask,
				info->PixelInformation.GreenMask,
				info->PixelInformation.BlueMask,
				info->PixelInformation.ReservedMask);
	}

	UINT32 Width = Gop->Mode->Info->HorizontalResolution;
	UINT32 Height = Gop->Mode->Info->VerticalResolution;

	// 背景を黒で塗りつぶし
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black = {0x00, 0x00, 0x00, 0x00};
	Gop->Blt(Gop, &Black, EfiBltVideoFill, 0, 0, 0, 0, Width, Height, 0);

	// 矩形を描画（例：赤い四角）
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL Red = {0x00, 0x00, 0xFF, 0x00};
	//UINT32 RectW = Width / 2;
	//UINT32 RectH = Height / 4;
	//UINT32 RectX = (Width - RectW) / 2;
	//UINT32 RectY = (Height - RectH) / 2;

	Gop->Blt(Gop, &Red, EfiBltVideoFill, 0, 0, 0, 0, Width, Height, 0);

	//
	// テキスト表示（UEFIのPrintを利用）
	//
	Print(L"Hello, Susanoo OS!\n");

	//
	// ===== ここからキー入力待ちの安定版 =====
	EFI_STATUS FlushKeys(void) {
		EFI_INPUT_KEY key;
		// バッファに残っているキーを全部読み捨てる
		while (gST->ConIn->ReadKeyStroke(gST->ConIn, &key) != EFI_NOT_READY) {
		//何もしない
		}
		return EFI_SUCCESS;
}

	void WaitAnyKey(void) {
		EFI_INPUT_KEY key;
		UINTN idx;

		// 念のため入力デバイスをリセット（古いイベントを掃除）
		// 第2引数はFALSEでOK
		gST->ConIn->Reset(gST->ConIn, FALSE);

		// さらにドレインして完全に空にする
		FlushKeys();

		Print(L"\nPress any key to exit...\n");

		// ブロッキングで「新しく押されたキー」を待つ
		for (;;) {
			// まずイベントでブロック
			gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &idx);
			// 次に実際のキーを読み出す（NOT_READYならもう一度待つ）
			if (!EFI_ERROR(gST->ConIn->ReadKeyStroke(gST->ConIn, &key))) {
				break;
			}
		}
	}
// ===== 置き換えここまで =====

	WaitAnyKey();
	return EFI_SUCCESS;
}
