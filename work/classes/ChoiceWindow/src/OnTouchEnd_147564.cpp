#include "class.h"

extern "C" int TouchButton_OnTouchEnd(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchEnd(void *self, int x, int y);

int ChoiceWindow::OnTouchEnd(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (this->f_10 != 0 && TouchButton_OnTouchEnd(this->f_10, x, y) != 0)
            return 0;
        if (this->f_14 != 0 && TouchButton_OnTouchEnd(this->f_14, x, y) != 0)
            return 1;
        if (this->f_18 != 0 && TouchButton_OnTouchEnd(this->f_18, x, y) != 0)
            return 2;
    }
    if (this->f_1c != 0)
        ScrollTouchWindow_OnTouchEnd(this->f_1c, x, y);
    return -1;
}
