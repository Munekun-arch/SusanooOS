#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL *gGop;

// ピクセル描画
VOID PutPixel(UINTN X, UINTN Y, UINT32 Color) {
    UINT32 *FrameBuffer = (UINT32*)gGop->Mode->FrameBufferBase;
    UINTN PixelsPerScanLine = gGop->Mode->Info->PixelsPerScanLine;
    FrameBuffer[Y * PixelsPerScanLine + X] = Color;
}

// 矩形塗りつぶし
VOID FillRect(UINTN X, UINTN Y, UINTN W, UINTN H, UINT32 Color) {
    for (UINTN j = 0; j < H; j++) {
        for (UINTN i = 0; i < W; i++) {
            PutPixel(X + i, Y + j, Color);
        }
    }
}

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    EFI_STATUS Status;

    // GOP プロトコル取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&gGop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        return Status;
    }

    // 画面サイズ取得
    UINTN Width  = gGop->Mode->Info->HorizontalResolution;
    UINTN Height = gGop->Mode->Info->VerticalResolution;

    // 背景を黒に
    FillRect(0, 0, Width, Height, 0x000000);

    // 赤い四角を中央に描画
    UINTN rectW = 200;
    UINTN rectH = 150;
    UINTN startX = (Width  - rectW) / 2;
    UINTN startY = (Height - rectH) / 2;
    FillRect(startX, startY, rectW, rectH, 0xFF0000);

    // 表示保持（キー入力待ち）
    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    EFI_INPUT_KEY Key;
    Print(L"\nPress any key to exit...\n");
    while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) {
        gBS->Stall(1000); // 1ms
    }

    return EFI_SUCCESS;
}

