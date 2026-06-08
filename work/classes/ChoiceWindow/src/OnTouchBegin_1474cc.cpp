#include "class.h"

extern "C" void TouchButton_OnTouchBegin(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchBegin(void *self, int x, int y);

int ChoiceWindow::OnTouchBegin(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (this->f_10 != 0)
            TouchButton_OnTouchBegin(this->f_10, x, y);
        if (this->f_14 != 0)
            TouchButton_OnTouchBegin(this->f_14, x, y);
        if (this->f_18 != 0)
            TouchButton_OnTouchBegin(this->f_18, x, y);
    }
    if (this->f_1c != 0)
        ScrollTouchWindow_OnTouchBegin(this->f_1c, x, y);
    return 0;
}
