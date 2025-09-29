#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/FileInfo.h>
#include "BmpLoader.h"

#pragma pack(push, 1)
typedef struct {
    UINT16 bfType;
    UINT32 bfSize;
    UINT16 bfReserved1;
    UINT16 bfReserved2;
    UINT32 bfOffBits;
} BMP_FILE_HEADER;

typedef struct {
    UINT32 biSize;
    INT32  biWidth;
    INT32  biHeight;
    UINT16 biPlanes;
    UINT16 biBitCount;
    UINT32 biCompression;
    UINT32 biSizeImage;
    INT32  biXPelsPerMeter;
    INT32  biYPelsPerMeter;
    UINT32 biClrUsed;
    UINT32 biClrImportant;
} BMP_INFO_HEADER;
#pragma pack(pop)

EFI_STATUS LoadBmpAndDraw(
    EFI_FILE_PROTOCOL *Root,
    CHAR16 *FileName,
    UINTN X,
    UINTN Y
) {
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL *File;

    // ファイルを開く
    Status = Root->Open(
        Root, &File, FileName,
        EFI_FILE_MODE_READ, 0
    );
    if (EFI_ERROR(Status)) {
        Print(L"Open %s failed!\n", FileName);
        return Status;
    }
    Print(L"Opened %s\n", FileName);

    // ヘッダ読み込み
    BMP_FILE_HEADER FileHeader;
    BMP_INFO_HEADER InfoHeader;
    UINTN Size;

    Size = sizeof(FileHeader);
    File->Read(File, &Size, &FileHeader);

    Size = sizeof(InfoHeader);
    File->Read(File, &Size, &InfoHeader);

    if (FileHeader.bfType != 0x4D42) { // "BM"
        Print(L"Not a BMP file!\n");
        File->Close(File);
        return EFI_ABORTED;
    }

    Print(L"BMP: %dx%d, %d bpp\n",
          InfoHeader.biWidth, InfoHeader.biHeight, InfoHeader.biBitCount);

    // ピクセルデータへシーク
    File->SetPosition(File, FileHeader.bfOffBits);

    // ピクセル読み込み
    UINTN BytesPerPixel = InfoHeader.biBitCount / 8;
    UINTN PixelBytes = InfoHeader.biWidth * InfoHeader.biHeight * BytesPerPixel;
    UINT8 *PixelData = AllocateZeroPool(PixelBytes);
    File->Read(File, &PixelBytes, PixelData);

    // GOP 取得
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        FreePool(PixelData);
        File->Close(File);
        return Status;
    }

    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    // 描画ループ
    for (INT32 y = 0; y < InfoHeader.biHeight; y++) {
        for (INT32 x = 0; x < InfoHeader.biWidth; x++) {
            UINT8* Src = &PixelData[(y * InfoHeader.biWidth + x) * BytesPerPixel];
            UINT32 Color;

            if (BytesPerPixel == 3) {
                // 24bit: BGR
                Color = (Src[2] << 16) | (Src[1] << 8) | Src[0];
            } else if (BytesPerPixel == 4) {
                // 32bit: BGRA
                Color = (Src[2] << 16) | (Src[1] << 8) | Src[0];
            } else {
                Color = 0xFFFFFF; // 未対応フォーマット
            }

            FrameBuffer[(Y + (InfoHeader.biHeight - 1 - y)) * Pitch + (X + x)] = Color;
        }
    }

    FreePool(PixelData);
    File->Close(File);
    return EFI_SUCCESS;
}

