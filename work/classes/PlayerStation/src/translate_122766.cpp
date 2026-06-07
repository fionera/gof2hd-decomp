#include "class.h"

extern "C" void AEGeometry_translate(void *geometry, float x, float y, float z);

void PlayerStation::translate(float x, float y, float z)
{
    return AEGeometry_translate(P(this, 0x140), x, y, z);
}
