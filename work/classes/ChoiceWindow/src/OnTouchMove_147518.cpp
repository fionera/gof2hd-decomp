#include "class.h"

extern "C" void TouchButton_OnTouchMove(void *self, int x, int y);
extern "C" void ScrollTouchWindow_OnTouchMove(void *self, int x, int y);

int ChoiceWindow::OnTouchMove(int x, int y)
{
    if (F<uint8_t>(this, 0x59) != 0) {
        if (this->f_10 != 0)
            TouchButton_OnTouchMove(this->f_10, x, y);
        if (this->f_14 != 0)
            TouchButton_OnTouchMove(this->f_14, x, y);
        if (this->f_18 != 0)
            TouchButton_OnTouchMove(this->f_18, x, y);
    }
    if (this->f_1c != 0)
        ScrollTouchWindow_OnTouchMove(this->f_1c, x, y);
    return 0;
}
