#include "class.h"

struct TouchSlider {
    int touchedInside(int param_1, int param_2);
};

int TouchSlider::touchedInside(int param_1, int param_2)
{
    int half = F<int>(this, 0x14) >> 1;
    int cx = F<int>(this, 0x8);
    int pad = F<int>(this, 0x38);
    if (cx - half - pad > param_1)
        return 0;
    if (param_1 >= half + cx + pad)
        return 0;
    int half2 = F<int>(this, 0x18) >> 1;
    int cy = F<int>(this, 0xc);
    if (cy - pad - half2 > param_2)
        return 0;
    return param_2 <= half2 + cy + pad;
}
