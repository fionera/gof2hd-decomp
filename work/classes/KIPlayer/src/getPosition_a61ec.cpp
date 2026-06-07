#include "class.h"

extern "C" void MatrixGetPosition(Vector *out, float *matrix);

// Returns the ship position by value (sret in r0, this in r1).
Vector KIPlayer_getPosition(KIPlayer *self)
{
    Vector v;
    MatrixGetPosition(&v, (float *)(*(char **)((char *)self + 0x4) + 0x4));
    return v;
}
