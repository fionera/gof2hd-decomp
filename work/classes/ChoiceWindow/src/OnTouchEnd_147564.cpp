#include "class.h"

extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);

int ChoiceWindow::OnTouchEnd(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (F<void *>(this, 0x10) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x10), x, y) != 0)
            return 0;
        if (F<void *>(this, 0x14) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x14), x, y) != 0)
            return 1;
        if (F<void *>(this, 0x18) != 0 && TouchButton_OnTouchEnd(F<void *>(this, 0x18), x, y) != 0)
            return 2;
    }
    if (F<void *>(this, 0x1c) != 0)
        ScrollTouchWindow_OnTouchEnd(F<void *>(this, 0x1c), x, y);
    return -1;
}
