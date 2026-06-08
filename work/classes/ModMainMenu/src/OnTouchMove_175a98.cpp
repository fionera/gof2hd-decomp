#include "class.h"

extern "C" void MenuTouchWindow_OnTouchMove(void *self, int x, int y, void *touch);

extern "C" __attribute__((disable_tail_calls)) void _ZN11ModMainMenu11OnTouchMoveEiiPv(
    ModMainMenu *self, int x, int y, void *touch)
{
    if (UC(self, 0x28) != 0)
        return;
    MenuTouchWindow_OnTouchMove(self->f_18, x, y, touch);
}
