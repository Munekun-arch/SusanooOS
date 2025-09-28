#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/GraphicsOutput.h>
#include <Guid/FileInfo.h>

#pragma pack(1)
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
#pragma pack()

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
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;

    // Graphics Output Protocol
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&Gop);
    if (EFI_ERROR(Status)) {
        Print(L"GOP not found!\n");
        return Status;
    }

    // LoadedImage
    Status = gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&LoadedImage);
    if (EFI_ERROR(Status)) return Status;

    // SimpleFS
    Status = gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID**)&SimpleFs);
    if (EFI_ERROR(Status)) return Status;

    Status = SimpleFs->OpenVolume(SimpleFs, &Root);
    if (EFI_ERROR(Status)) return Status;

    // BMP ファイルを開く
    Status = Root->Open(
    Root,
    &File,
    L"EFI\\BOOT\\test.bmp",
    EFI_FILE_MODE_READ,
    0
	);
    if (EFI_ERROR(Status)) {
        Print(L"Open test.bmp failed!\n");
        return Status;
    }

    // ファイルサイズ取得
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    FileInfo = AllocateZeroPool(FileInfoSize);
    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    UINTN FileSize = FileInfo->FileSize;
    FreePool(FileInfo);

    // 読み込み
    UINT8 *Buffer = AllocateZeroPool(FileSize);
    Status = File->Read(File, &FileSize, Buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Read BMP failed!\n");
        return Status;
    }
    File->Close(File);

    // ヘッダ取得
    BMP_FILE_HEADER *FileHdr = (BMP_FILE_HEADER*)Buffer;
    BMP_INFO_HEADER *InfoHdr = (BMP_INFO_HEADER*)(Buffer + sizeof(BMP_FILE_HEADER));

    if (FileHdr->bfType != 0x4D42) {
        Print(L"Not a BMP file!\n");
        return EFI_ABORTED;
    }

    UINT8 *PixelData = Buffer + FileHdr->bfOffBits;
    INT32 Width = InfoHdr->biWidth;
    INT32 Height = InfoHdr->biHeight;
    UINT16 BitCount = InfoHdr->biBitCount;

    Print(L"BMP: %dx%d %dbit\n", Width, Height, BitCount);

    // 描画 (24bit RGB想定)
    for (INT32 y = 0; y < Height; y++) {
        for (INT32 x = 0; x < Width; x++) {
            UINT8 *Pixel = PixelData + (y * Width + x) * 3;
            UINT32 Color = (Pixel[2] << 16) | (Pixel[1] << 8) | Pixel[0]; // BGR→RGB

            UINT32 *FbBase = (UINT32*)Gop->Mode->FrameBufferBase;
            UINTN Pitch = Gop->Mode->Info->PixelsPerScanLine;
            FbBase[(Height - 1 - y) * Pitch + x] = Color; // 上下反転
        }
    }

    Print(L"BMP drawn!\n");

    // キー入力待ち
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

