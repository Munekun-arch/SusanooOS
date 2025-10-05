#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <Uefi.h>
#include "Window.h"

#define MAX_WINDOWS 16

typedef struct {
    WINDOW* Windows[MAX_WINDOWS];
    UINTN Count;
} WINDOW_MANAGER;

VOID InitWindowManager(WINDOW_MANAGER* wm);
BOOLEAN AddWindow(WINDOW_MANAGER* wm, WINDOW* win);
VOID DrawAllWindows(EFI_GRAPHICS_OUTPUT_PROTOCOL* Gop, WINDOW_MANAGER* wm);

#endif

