#include "class.h"

void ScrollTouchBox::setPosition(int x, int y)
{
    F<int>(this, 0x4) = x;
    F<int>(this, 0x8) = y;
}
