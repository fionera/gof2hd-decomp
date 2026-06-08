#include "class.h"

__attribute__((visibility("hidden"))) extern int *g_ModMainMenu_touchEndFlag;

extern "C" void MenuTouchWindow_OnTouchEnd(void *self, int x, int y, void *touch);
extern "C" void *Level_getStarSystem(void *level);
extern "C" void ModMainMenu_touchEndTail(void *starSystem);

extern "C" void _ZN11ModMainMenu10OnTouchEndEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) == 0) {
        MenuTouchWindow_OnTouchEnd(self->f_18, x, y, touch);
        ModMainMenu_touchEndTail(Level_getStarSystem(*(void **)self->f_1c));
        return;
    }

    UC(self, 0x28) = 0;
    *g_ModMainMenu_touchEndFlag = 0;
}
