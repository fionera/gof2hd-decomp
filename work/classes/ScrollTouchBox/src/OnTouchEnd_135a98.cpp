#include "class.h"

void ScrollTouchBox::OnTouchEnd(int x, int y)
{
    if (this->f_30 != 0) {
        int delta = this->f_1c;
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float)delta;
        this->f_20 = 0.95f;
        this->f_30 = 0;
        this->f_34 = delta + this->f_34;
        this->f_24 = speed;
    }
}
