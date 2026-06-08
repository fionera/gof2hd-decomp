#include "class.h"

extern "C" void AEGeometry_setPosition(AEGeometry *geometry, const Vector *position);

void PlayerAsteroid::setPosition(const Vector &position)
{
    return AEGeometry_setPosition(F<AEGeometry *>(this, 0x8), &position);
}
