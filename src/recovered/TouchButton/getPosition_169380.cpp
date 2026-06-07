#include "class.h"

struct Vec3 { float x, y, z; };

extern "C" void TouchButton_getPosition(Vec3 *out, TouchButton *self)
{
    float x = (float)I(self, 0x78);
    float y = (float)I(self, 0x7c);
    out->z = 0;
    out->y = y;
    out->x = x;
}
