#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain (
	IN EFI_HANDLE				ImageHandle,
	IN EFI_SYSTEM_TABLE	*SystemTable
	)
{
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

	// GOPの取得
	Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
	if (EFI_ERROR(Status)) {
		Print(L"Failed to locate GOP: %r\n", Status);
		return Status;
	}

	// 背景塗り潰し（黒）
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL Black = {0x00, 0x00, 0x00, 0x00};
	Gop->Blt(
		Gop,
		&Black,
		EfiBltVideoFill,
		0,
		0,
		0,
		0,
		Gop->Mode->Info->HorizontalResolution,
		Gop->Mode->Info->VerticalResolution,
		0
	);

	// テキスト表示
	Print(L"Hello, Susanoo OS!\n");

	// キー入力待ち（画面がすぐ閉じないように）
	SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	Print(L"Press any key to exit...\n");
	EFI_INPUT_KEY Key;
	SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

	return EFI_SUCCESS;
}
