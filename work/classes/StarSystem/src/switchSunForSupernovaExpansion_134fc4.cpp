#include "class.h"

extern "C" void AEGeometry_setScaling(AEGeometry *geom, const char *scale);

extern "C" void StarSystem_switchSunForSupernovaExpansion(StarSystem *self)
{
    char scaleBytes[12];
    AEGeometry *geom = *(AEGeometry **)P(P(self, 0x1c), 4);
    uint32_t *scale = (uint32_t *)scaleBytes;
    scale[0] = 0x46311048;
    scale[1] = 0x46311048;
    scale[2] = 0x46311048;
    AEGeometry_setScaling(geom, scaleBytes);
}
