#include "class.h"

struct TouchSlider {
    bool OnTouchMove(int param_1, int param_2);
};

bool TouchSlider::OnTouchMove(int param_1, int param_2)
{
    if (F<uint8_t>(this, 0x35) != 0)
        return false;
    if (F<uint8_t>(this, 0x34) != 0) {
        int half = F<int>(this, 0x14) / 2;
        int lo = F<int>(this, 0x0) + half;
        int hi = (F<int>(this, 0x0) + F<int>(this, 0x1c)) - half;
        if (param_1 < hi)
            hi = param_1;
        if (hi <= lo)
            hi = lo;
        F<int>(this, 0x8) = hi;
    }
    return F<uint8_t>(this, 0x34) != 0;
}
