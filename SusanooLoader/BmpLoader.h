#ifndef _BMP_LOADER_H_
#define _BMP_LOADER_H_

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS
LoadBmpAndDraw(
    EFI_FILE_PROTOCOL *Root,
    CHAR16 *FileName,
    INTN DestX,
    INTN DestY
);

#endif

