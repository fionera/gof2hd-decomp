#include "class.h"

struct Vec3 {
    double xy;
    float z;
};

extern "C" void Player_getHitVector(Vec3 *out, Player *self)
{
    double xy = *(double *)self->hitVector;
    out->z = self->hitVector[2];
    out->xy = xy;
}
