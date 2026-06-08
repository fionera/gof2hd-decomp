#include "class.h"

float ScrollTouchBox::getRelativeScrollHeight()
{
    int height = this->f_10;
    int contentHeight = this->f_18;
    if (height > contentHeight)
        return 1.0f;

    int pos = this->f_34;
    if (pos >= 1) {
        pos = height - pos;
    } else if (pos < height - contentHeight) {
        pos = pos + contentHeight;
    } else {
        pos = height;
    }

    return (float)pos / (float)contentHeight;
}
