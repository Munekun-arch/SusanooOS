#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_STATUS Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&Gop);
    if (EFI_ERROR(Status)) return Status;

    UINT32 Width = Gop->Mode->Info->HorizontalResolution;
    UINT32 Height = Gop->Mode->Info->VerticalResolution;
    UINT32 *FB = (UINT32 *)Gop->Mode->FrameBufferBase;
    UINT32 Pitch = Gop->Mode->Info->PixelsPerScanLine;

    // アニメーション: 四角が右に動く
    for (int x = 0; x < (int)Width - 100; x += 10) {
        // 背景を黒でクリア
        for (UINT32 y = 0; y < Height; y++) {
            for (UINT32 xx = 0; xx < Width; xx++) {
                FB[y * Pitch + xx] = 0x000000; // 黒
            }
        }

        // 四角を描画
        for (UINT32 y = 100; y < 200; y++) {
            for (UINT32 xx = x; xx < x + 100; xx++) {
                FB[y * Pitch + xx] = 0xFF0000; // 赤
            }
        }

        // 少し待つ
        gBS->Stall(50000); // 0.05秒
    }

    // 終了待ち（キー入力待ち）
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gST->ConIn->Reset(gST->ConIn, FALSE);
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

