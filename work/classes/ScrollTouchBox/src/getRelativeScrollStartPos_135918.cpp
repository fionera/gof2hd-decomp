#include "class.h"

float ScrollTouchBox::getRelativeScrollStartPos()
{
    int pos = F<int>(this, 0x34);
    if (pos > 0)
        return 0.0f;
    return -(float)pos / (float)F<int>(this, 0x18);
}
