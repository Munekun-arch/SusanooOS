#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

// 8x16 フォントパターン (例: 'A')
static const UINT8 Font_A[16] = {
    0x18, //    XX
    0x24, //   X  X
    0x42, //  X    X
    0x42, //  X    X
    0x7E, //  XXXXXX
    0x42, //  X    X
    0x42, //  X    X
    0x42, //  X    X
    0x42, //  X    X
    0x42, //  X    X
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00  //
};

// ピクセルを描画
STATIC
VOID
DrawPixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINT32 Color) {
    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    FrameBuffer[Y * Pitch + X] = Color;
}

// 文字 'A' を描画
STATIC
VOID
DrawCharA(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINTN X, UINTN Y, UINT32 Color) {
    for (UINTN row = 0; row < 16; row++) {
        UINT8 bits = Font_A[row];
        for (UINTN col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col))) {
                DrawPixel(Gop, X + col, Y + row, Color);
            }
        }
    }
}

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_STATUS Status;

    // GOP を取得
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        return Status;
    }

    // 背景を黒で塗る
    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
    UINTN Width = Gop->Mode->Info->HorizontalResolution;
    UINTN Height = Gop->Mode->Info->VerticalResolution;

    for (UINTN y = 0; y < Height; y++) {
        for (UINTN x = 0; x < Width; x++) {
            FrameBuffer[y * Pitch + x] = 0x000000; // 黒
        }
    }

    // 文字 'A' を描画
    DrawCharA(Gop, 100, 100, 0xFFFFFF); // 白色の 'A'

    // キー入力待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

