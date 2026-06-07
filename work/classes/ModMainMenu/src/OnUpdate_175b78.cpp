#include "class.h"

__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateLayout;
__attribute__((visibility("hidden"))) extern void **g_ModMainMenu_updateListener;

extern "C" int ApplicationManager_GetElapsedTimeMillis(void *app);
extern "C" void Layout_update(void *layout, int elapsed);
extern "C" void CutScene_update(void *scene, int elapsed);
extern "C" void MenuTouchWindow_update(void *window, int elapsed);
extern "C" void FModSound_updateAll(
    void *listener, void *position, void *velocity, void *forward, void *up);

extern "C" void _ZN11ModMainMenu8OnUpdateEv(ModMainMenu *self)
{
    int elapsed = ApplicationManager_GetElapsedTimeMillis(P(self, 0x08));
    int frameTime;
    if (elapsed < 0x97 &&
        (elapsed = ApplicationManager_GetElapsedTimeMillis(P(self, 0x08))) < 0) {
        frameTime = 0;
    } else {
        elapsed = ApplicationManager_GetElapsedTimeMillis(P(self, 0x08));
        if (elapsed > 0x96)
            frameTime = 0x96;
        else
            frameTime = ApplicationManager_GetElapsedTimeMillis(P(self, 0x08));
    }

    I(self, 0x14) = frameTime;

    void **layout = g_ModMainMenu_updateLayout;
    Layout_update(*layout, frameTime);
    CutScene_update(P(self, 0x1c), I(self, 0x14));
    if (UC(self, 0x28) == 0)
        MenuTouchWindow_update(P(self, 0x18), I(self, 0x14));
    Layout_update(*layout, I(self, 0x14));

    void **listener = g_ModMainMenu_updateListener;
    I(self, 0x24) = I(self, 0x14) + I(self, 0x24);
    void *zero = 0;
    FModSound_updateAll(*listener, zero, zero, zero, zero);
}
