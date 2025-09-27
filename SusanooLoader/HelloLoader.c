#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_STATUS Status;

    // GOP 取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"Unable to locate GOP\n");
        return Status;
    }

    UINT32 Width  = Gop->Mode->Info->HorizontalResolution;
    UINT32 Height = Gop->Mode->Info->VerticalResolution;
    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;

    // 横方向グラデーション (赤→青)
    for (UINT32 y = 0; y < Height; y++) {
        for (UINT32 x = 0; x < Width; x++) {
            UINT8 ratio = (x * 255) / Width;
            UINT8 r = 255 - ratio;
            UINT8 g = 0;
            UINT8 b = ratio;
            UINT32 color = (r << 16) | (g << 8) | b;
            FrameBuffer[y * (Gop->Mode->Info->PixelsPerScanLine) + x] = color;
        }
    }

    Print(L"Gradient Drawn! Press any key to exit...\n");

    // 🔽 ここで待機する
    EFI_INPUT_KEY Key;
    SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
    while (TRUE) {
        Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
        if (!EFI_ERROR(Status)) {
            break;  // 何かキーが押されたら抜ける
        }
    }

    return EFI_SUCCESS;
}

