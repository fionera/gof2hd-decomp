#include "class.h"

__attribute__((visibility("hidden"))) extern Vector *PlayerAsteroid_center;
__attribute__((visibility("hidden"))) extern int *PlayerAsteroid_centerLength;

extern "C" Vector *Vector_assign(Vector *self, const Vector *other);
extern "C" float VectorLength(const Vector *vector);

void PlayerAsteroid::setAsteroidCenter(Vector center)
{
    volatile char localBytes[13];
    Vector *local = (Vector *)(void *)localBytes;
    *local = center;
    Vector_assign(PlayerAsteroid_center, local);
    *PlayerAsteroid_centerLength = (int)VectorLength(local);
}
