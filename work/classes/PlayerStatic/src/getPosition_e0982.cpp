#include "class.h"

extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);

Vector PlayerStatic::getPosition()
{
    AEGeometry *geometry = (AEGeometry *)this->f_8;
    if (geometry != 0) {
        return AEGeometry_getPosition(geometry);
    }

    Vector result;
    result.x = (float)this->f_124;
    result.y = (float)this->f_128;
    result.z = (float)this->f_12c;
    return result;
}
