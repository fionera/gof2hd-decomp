#include "class.h"

struct TouchSlider {
    void setValue(float param_1);
};

void TouchSlider::setValue(float param_1)
{
    float fVar1 = (float)(F<int>(this, 0x1c) - F<int>(this, 0x14));
    float fVar2 = (float)(F<int>(this, 0x0) + F<int>(this, 0x14) / 2);
    F<int>(this, 0x8) = (int)(fVar2 + fVar1 * param_1);
}
