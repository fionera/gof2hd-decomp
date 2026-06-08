#include "class.h"

void ScrollTouchBox::OnTouchBegin(int x, int y)
{
    if (touchIsInside(x, y)) {
        F<int>(this, 0x28) = y;
        F<int>(this, 0x2c) = y;
        F<int>(this, 0x1c) = 0;
        F<uint8_t>(this, 0x30) = 1;
    }
}
