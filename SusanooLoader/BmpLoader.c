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
    INTN DestX,
    INTN DestY
) {
    EFI_FILE_PROTOCOL *File;
    EFI_STATUS Status;

    Status = Root->Open(Root, &File, FileName, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(Status)) {
        Print(L"Open %s failed!\n", FileName);
        return Status;
    }

    BMP_FILE_HEADER FileHeader;
    BMP_INFO_HEADER InfoHeader;
    UINTN Size;

    Size = sizeof(FileHeader);
    File->Read(File, &Size, &FileHeader);
    Size = sizeof(InfoHeader);
    File->Read(File, &Size, &InfoHeader);

    if (FileHeader.bfType != 0x4D42) {
        Print(L"Not a BMP file!\n");
        File->Close(File);
        return EFI_ABORTED;
    }

    if (InfoHeader.biBitCount != 24 && InfoHeader.biBitCount != 32) {
        Print(L"Unsupported BMP bit depth: %d\n", InfoHeader.biBitCount);
        File->Close(File);
        return EFI_ABORTED;
    }

    File->SetPosition(File, FileHeader.bfOffBits);
    UINTN RowBytes = ((InfoHeader.biWidth * InfoHeader.biBitCount + 31) / 32) * 4;
    UINTN PixelBytes = RowBytes * ABS(InfoHeader.biHeight);
    UINT8 *PixelData = AllocateZeroPool(PixelBytes);
    File->Read(File, &PixelBytes, PixelData);

    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        FreePool(PixelData);
        return Status;
    }

    UINT32* FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    for (INT32 y = 0; y < ABS(InfoHeader.biHeight); y++) {
        for (INT32 x = 0; x < InfoHeader.biWidth; x++) {
            UINT8* Src;

            if (InfoHeader.biBitCount == 24) {
                Src = &PixelData[y * RowBytes + x * 3];
                UINT32 Color = (Src[2] << 16) | (Src[1] << 8) | Src[0];
                FrameBuffer[(DestY + (InfoHeader.biHeight > 0 ? (ABS(InfoHeader.biHeight)-1-y) : y)) * Pitch + (DestX + x)] = Color;
            } else {
                Src = &PixelData[y * RowBytes + x * 4];
                UINT32 Color = (Src[2] << 16) | (Src[1] << 8) | Src[0];
                FrameBuffer[(DestY + (InfoHeader.biHeight > 0 ? (ABS(InfoHeader.biHeight)-1-y) : y)) * Pitch + (DestX + x)] = Color;
            }
        }
    }

    FreePool(PixelData);
    File->Close(File);

    Print(L"%s drawn at (%d,%d)\n", FileName, DestX, DestY);
    return EFI_SUCCESS;
}

