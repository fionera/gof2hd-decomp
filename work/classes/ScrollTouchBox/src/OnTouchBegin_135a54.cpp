#include "class.h"

void ScrollTouchBox::OnTouchBegin(int x, int y)
{
    if (touchIsInside(x, y)) {
        this->f_28 = y;
        this->f_2c = y;
        this->f_1c = 0;
        this->f_30 = 1;
    }
}
