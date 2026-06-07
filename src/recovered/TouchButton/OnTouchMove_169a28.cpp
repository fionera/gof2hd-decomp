#include "class.h"

extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py);

extern "C" unsigned int TouchButton_OnTouchMove(TouchButton *self, int px, int py)
{
    if (UC(self, 0xb2) != 0 && UC(self, 0xb3) == 0) {
        unsigned int r;
        if (UC(self, 0xb0) == 0)
            r = 0;
        else
            r = TouchButton_touchedInside(self, px, py);
        UC(self, 0xb0) = (unsigned char)r;
        return r;
    }
    return 0;
}
