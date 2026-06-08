#include "class.h"

// sret in r0, this in r1. Reads three signed ints at +0x178/0x17c/0x180 and converts to float.
struct PosOut { float x, y, z; };

extern "C" void PlayerFixedObject_getPosition(PosOut *out, PlayerFixedObject *self)
{
    float a = (float)self->f_178;
    float b = (float)self->f_17c;
    float c = (float)self->f_180;
    out->x = a;
    out->y = b;
    out->z = c;
}
