#include "class.h"

struct TouchSlider {
    float getValue();
};

float TouchSlider::getValue()
{
    float fVar1 = (float)(F<int>(this, 0x1c) - F<int>(this, 0x14));
    float fVar2 = (float)((F<int>(this, 0x8) - F<int>(this, 0x14) / 2) - F<int>(this, 0x0));
    return fVar2 / fVar1;
}
