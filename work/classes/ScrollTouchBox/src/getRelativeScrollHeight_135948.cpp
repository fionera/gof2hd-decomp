#include "class.h"

float ScrollTouchBox::getRelativeScrollHeight()
{
    int height = F<int>(this, 0x10);
    int contentHeight = F<int>(this, 0x18);
    if (height > contentHeight)
        return 1.0f;

    int pos = F<int>(this, 0x34);
    if (pos >= 1) {
        pos = height - pos;
    } else if (pos < height - contentHeight) {
        pos = pos + contentHeight;
    } else {
        pos = height;
    }

    return (float)pos / (float)contentHeight;
}
