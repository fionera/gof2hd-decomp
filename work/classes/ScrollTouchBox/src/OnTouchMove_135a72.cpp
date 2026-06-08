#include "class.h"

void ScrollTouchBox::OnTouchMove(int x, int y)
{
    if (this->f_30 != 0 && this->f_18 > this->f_10) {
        int delta = y - this->f_2c;
        this->f_2c = y;
        this->f_1c = delta;
        this->f_20 = 1.0f;
        this->f_34 = delta + this->f_34;
    }
}
