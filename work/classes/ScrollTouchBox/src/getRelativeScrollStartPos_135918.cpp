#include "class.h"

float ScrollTouchBox::getRelativeScrollStartPos()
{
    int pos = this->f_34;
    if (pos > 0)
        return 0.0f;
    return -(float)pos / (float)this->f_18;
}
