#include "class.h"

extern "C" Vector AEGeometry_getPosition(void *geometry);

Vector PlayerStation::getPosition()
{
    return AEGeometry_getPosition(P(this, 0x140));
}
