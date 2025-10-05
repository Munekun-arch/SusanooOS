#include "WindowManager.h"
#include "Graphics.h"

VOID InitWindowManager(WINDOW_MANAGER* wm) {
    wm->Count = 0;
    for (UINTN i = 0; i < MAX_WINDOWS; i++) {
        wm->Windows[i] = NULL;
    }
}

BOOLEAN AddWindow(WINDOW_MANAGER* wm, WINDOW* win) {
    if (wm->Count >= MAX_WINDOWS) return FALSE;
    wm->Windows[wm->Count++] = win;
    return TRUE;
}

VOID DrawAllWindows(EFI_GRAPHICS_OUTPUT_PROTOCOL* Gop, WINDOW_MANAGER* wm) {
    for (UINTN i = 0; i < wm->Count; i++) {
        DrawWindow(Gop, wm->Windows[i]);
    }
}

