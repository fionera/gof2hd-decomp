#include "class.h"

void ScrollTouchBox::update(int dt)
{
    int height = F<int>(this, 0x10);
    int contentHeight = F<int>(this, 0x18);
    if (contentHeight < height)
        return;

    if (F<uint8_t>(this, 0x30) == 0) {
        float velocity = F<float>(this, 0x20) * F<float>(this, 0x24);
        float absVelocity = -velocity;
        if (velocity > 0.0f)
            absVelocity = velocity;
        F<float>(this, 0x24) = velocity;
        if (absVelocity > 1.5f)
            F<int>(this, 0x34) = (int)(velocity + (float)F<int>(this, 0x34));
    }

    int pos = F<int>(this, 0x34);
    int pull;
    if (pos < 1)
        goto negative;

    pull = -pos;
    goto apply;

negative:
    {
        int over = contentHeight - height;
        int min = -over;
        if (min <= pos)
            return;
        over += pos;
        pull = -over;
    }

apply:
    F<float>(this, 0x20) = 1.0f;
    F<float>(this, 0x24) = (float)pull * 0.5f;
}
