#include "class.h"

// sret in r0, this in r1. Reads three signed ints at +0x178/0x17c/0x180 and converts to float.
struct PosOut { float x, y, z; };

extern "C" void PlayerFixedObject_getPosition(PosOut *out, PlayerFixedObject *self)
{
    float a = (float)F<int32_t>(self, 0x178);
    float b = (float)F<int32_t>(self, 0x17c);
    float c = (float)F<int32_t>(self, 0x180);
    out->x = a;
    out->y = b;
    out->z = c;
}
