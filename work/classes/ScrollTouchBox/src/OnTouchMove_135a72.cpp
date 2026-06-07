#include "class.h"

void ScrollTouchBox::OnTouchMove(int x, int y)
{
    if (F<uint8_t>(this, 0x30) != 0 && F<int>(this, 0x18) > F<int>(this, 0x10)) {
        int delta = y - F<int>(this, 0x2c);
        F<int>(this, 0x2c) = y;
        F<int>(this, 0x1c) = delta;
        F<float>(this, 0x20) = 1.0f;
        F<int>(this, 0x34) = delta + F<int>(this, 0x34);
    }
}
