#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS
LoadBmpAndDraw(
    EFI_FILE_PROTOCOL *Root,
    CHAR16 *FileName,
    INTN X,
    INTN Y
);

#endif

