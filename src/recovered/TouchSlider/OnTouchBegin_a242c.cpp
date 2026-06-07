#include "class.h"

struct TouchSlider {
    int OnTouchBegin(int param_1, int param_2);
    int touchedInside(int param_1, int param_2);
};

int TouchSlider::OnTouchBegin(int param_1, int param_2)
{
    if (F<uint8_t>(this, 0x35) != 0)
        return 0;
    int r = touchedInside(param_1, param_2);
    F<uint8_t>(this, 0x34) = (uint8_t)r;
    return r;
}
