#include "class.h"

extern "C" Vector AEGeometry_getPosition(AEGeometry *geometry);

Vector PlayerStatic::getPosition()
{
    AEGeometry *geometry = (AEGeometry *)pp(this, 0x8);
    if (geometry != 0) {
        return AEGeometry_getPosition(geometry);
    }

    Vector result;
    result.x = (float)i32(this, 0x124);
    result.y = (float)i32(this, 0x128);
    result.z = (float)i32(this, 0x12c);
    return result;
}
