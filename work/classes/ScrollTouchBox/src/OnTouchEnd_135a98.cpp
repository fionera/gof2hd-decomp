#include "class.h"

void ScrollTouchBox::OnTouchEnd(int x, int y)
{
    if (F<uint8_t>(this, 0x30) != 0) {
        int delta = F<int>(this, 0x1c);
        float speed = 0.0f;
        if ((delta < 0 ? -delta : delta) > 3)
            speed = (float)delta;
        F<float>(this, 0x20) = 0.95f;
        F<uint8_t>(this, 0x30) = 0;
        F<int>(this, 0x34) = delta + F<int>(this, 0x34);
        F<float>(this, 0x24) = speed;
    }
}
