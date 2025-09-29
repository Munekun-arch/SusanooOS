#ifndef _BMP_LOADER_H_
#define _BMP_LOADER_H_

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>

// BMP を読み込み、指定座標に描画する
EFI_STATUS
LoadBmpAndDraw(
    EFI_FILE_PROTOCOL *Root,
    CHAR16 *FileName,
    UINTN X,
    UINTN Y
);

#endif

