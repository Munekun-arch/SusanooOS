#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/FileInfo.h>

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

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs;
    EFI_FILE_PROTOCOL *Root, *File;

    // LoadedImage を取得
    Status = gBS->HandleProtocol(
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&LoadedImage
    );
    if (EFI_ERROR(Status)) {
        Print(L"LoadedImage not found!\n");
        return Status;
    }

    // SimpleFS を取得
    Status = gBS->HandleProtocol(
        LoadedImage->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&SimpleFs
    );
    if (EFI_ERROR(Status)) {
        Print(L"SimpleFS not found!\n");
        return Status;
    }

    // ルートディレクトリを開く
    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) {
        Print(L"OpenVolume failed!\n");
        return Status;
    }

    // BMP ファイルを開く
    Status = Root->Open(
        Root,
        &File,
        L"\\EFI\\BOOT\\test.bmp",
        EFI_FILE_MODE_READ,
        0
    );
    if (EFI_ERROR(Status)) {
        Print(L"Open test.bmp failed!\n");
        return Status;
    }

    Print(L"File opened successfully!\n");

    // BMP ヘッダを読む
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

    Print(L"BMP: %dx%d, %d bpp\n", InfoHeader.biWidth, InfoHeader.biHeight, InfoHeader.biBitCount);

    // ピクセルデータへシーク
    File->SetPosition(File, FileHeader.bfOffBits);

    // 行サイズ（4バイト境界揃え）
    UINTN RowSize = ((InfoHeader.biBitCount * InfoHeader.biWidth + 31) / 32) * 4;
    UINTN PixelBytes = RowSize * InfoHeader.biHeight;

    UINT8 *PixelData = AllocateZeroPool(PixelBytes);
    File->Read(File, &PixelBytes, PixelData);

    // GOP 取得
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        return Status;
    }

    UINT32 *FrameBuffer = (UINT32*)Gop->Mode->FrameBufferBase;
    UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;

    // 描画ループ
    for (INT32 y = 0; y < InfoHeader.biHeight; y++) {
        UINT8 *Row = PixelData + (RowSize * y);
        for (INT32 x = 0; x < InfoHeader.biWidth; x++) {
            UINT8 *Src = &Row[x * (InfoHeader.biBitCount / 8)];
            UINT32 Color;

            if (InfoHeader.biBitCount == 24) {
                Color = (Src[2] << 16) | (Src[1] << 8) | Src[0];
            } else if (InfoHeader.biBitCount == 32) {
                Color = (Src[2] << 16) | (Src[1] << 8) | Src[0]; // alpha無視
            } else {
                continue;
            }

            // BMP は bottom-up 格納なので上下反転
            INT32 DrawY = (InfoHeader.biHeight - 1 - y);
            FrameBuffer[(100 + DrawY) * Pitch + (100 + x)] = Color;
        }
    }

    FreePool(PixelData);
    File->Close(File);

    // キー入力待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

