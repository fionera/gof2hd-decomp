#include "class.h"

extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);

Vector PlayerAsteroid::getPosition()
{
    return AEGeometry_getPosition(F<AEGeometry *>(this, 0x8));
}
