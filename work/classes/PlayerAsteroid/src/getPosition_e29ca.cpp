#include "class.h"

extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);

Vector PlayerAsteroid::getPosition()
{
    return AEGeometry_getPosition(this->f_8);
}
