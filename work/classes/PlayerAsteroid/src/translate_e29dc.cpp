#include "class.h"

extern "C" void AEGeometry_translate(AEGeometry *geometry, const Vector *delta);

void PlayerAsteroid::translate(const Vector &delta)
{
    return AEGeometry_translate(F<AEGeometry *>(this, 0x8), &delta);
}
