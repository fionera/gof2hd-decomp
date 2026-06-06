#include "class.h"

extern "C" float *Player_setHitVector(Player *self, float x, float y, float z)
{
    float *p = self->hitVector;
    *p++ = x;
    *p++ = y;
    *p++ = z;
    return p;
}
