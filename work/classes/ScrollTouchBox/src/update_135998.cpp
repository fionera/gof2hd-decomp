#include "class.h"

void ScrollTouchBox::update(int dt)
{
    int height = this->f_10;
    int contentHeight = this->f_18;
    if (contentHeight < height)
        return;

    if (this->f_30 == 0) {
        float velocity = this->f_20 * this->f_24;
        float absVelocity = -velocity;
        if (velocity > 0.0f)
            absVelocity = velocity;
        this->f_24 = velocity;
        if (absVelocity > 1.5f)
            this->f_34 = (int)(velocity + (float)this->f_34);
    }

    int pos = this->f_34;
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
    this->f_20 = 1.0f;
    this->f_24 = (float)pull * 0.5f;
}
