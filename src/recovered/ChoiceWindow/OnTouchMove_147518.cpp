#include "class.h"

extern "C" void TouchButton_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);

int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x10), x, y);
        if (F<void *>(this, 0x14) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x14), x, y);
        if (F<void *>(this, 0x18) != 0)
            TouchButton_OnTouchMove(F<void *>(this, 0x18), x, y);
    }
    if (F<void *>(this, 0x1c) != 0)
        ScrollTouchWindow_OnTouchMove(F<void *>(this, 0x1c), x, y);
    return 0;
}
