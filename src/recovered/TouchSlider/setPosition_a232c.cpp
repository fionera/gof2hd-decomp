#include "class.h"

struct TouchSlider {
    void setPosition(int param_1, int param_2);
};

void TouchSlider::setPosition(int param_1, int param_2)
{
    float fVar1 = (float)(F<int>(this, 0x1c) - F<int>(this, 0x14));
    float fVar2 = (float)(param_1 + F<int>(this, 0x14) / 2);
    F<int>(this, 0x0) = param_1;
    F<int>(this, 0x4) = param_2;
    F<int>(this, 0xc) = param_2 + F<int>(this, 0x20) / 2;
    F<int>(this, 0x8) = (int)(fVar2 + F<float>(this, 0x24) * fVar1);
}
