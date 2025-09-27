#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>

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

    // test.txt を開く
    Status = Root->Open(
        Root,
        &File,
        L"test.txt",
        EFI_FILE_MODE_READ,
        0
    );
    if (EFI_ERROR(Status)) {
        Print(L"Open test.txt failed!\n");
        return Status;
    }

    // ファイルサイズを取得
    EFI_FILE_INFO *FileInfo;
    UINTN FileInfoSize = sizeof(EFI_FILE_INFO) + 200;
    FileInfo = AllocateZeroPool(FileInfoSize);

    Status = File->GetInfo(File, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);
    if (EFI_ERROR(Status)) {
        Print(L"GetInfo failed!\n");
        return Status;
    }

    UINTN FileSize = FileInfo->FileSize;
    FreePool(FileInfo);

    // バッファ確保して読み込み
    CHAR8 *Buffer = AllocateZeroPool(FileSize + 1);
    Status = File->Read(File, &FileSize, Buffer);
    if (EFI_ERROR(Status)) {
        Print(L"Read failed!\n");
        return Status;
    }

    // ASCII → Unicode に変換して表示
    CHAR16 *UnicodeBuf = AllocateZeroPool((FileSize + 1) * sizeof(CHAR16));
    for (UINTN i = 0; i < FileSize; i++) {
        UnicodeBuf[i] = (CHAR16)Buffer[i];
    }
    Print(L"Contents of test.txt: %s\n", UnicodeBuf);

    // 後処理
    FreePool(Buffer);
    FreePool(UnicodeBuf);
    File->Close(File);

    // キー入力待ち
    Print(L"Press any key to exit...\n");
    EFI_INPUT_KEY Key;
    UINTN EventIndex;
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

    return EFI_SUCCESS;
}

