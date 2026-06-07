#include "class.h"

struct TouchSlider {
    void setHalfTransparent(bool param_1);
};

void TouchSlider::setHalfTransparent(bool param_1)
{
    F<uint8_t>(this, 0x35) = param_1;
}
