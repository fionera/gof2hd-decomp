#include "class.h"

extern "C" void MenuTouchWindow_OnTouchBegin(void *self, int x, int y, void *touch);

extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu12OnTouchBeginEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) != 0)
        return;
    MenuTouchWindow_OnTouchBegin(P(self, 0x18), x, y, touch);
}
