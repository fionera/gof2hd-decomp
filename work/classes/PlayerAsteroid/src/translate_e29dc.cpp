#include "class.h"

extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);

void PlayerAsteroid::translate(const Vector &delta)
{
    return AEGeometry_translate(this->f_8, &delta);
}
