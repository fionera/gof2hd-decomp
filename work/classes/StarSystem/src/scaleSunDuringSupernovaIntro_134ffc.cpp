#include "class.h"

extern "C" void AEGeometry_getScaling(char *out, AEGeometry *geom);
extern "C" void AEGeometry_setScaling3(AEGeometry *geom, float x, float y, float z);

extern "C" void StarSystem_scaleSunDuringSupernovaIntro(StarSystem *self, int amount)
{
    char scaleBytes[12];
    AEGeometry_getScaling(scaleBytes, *(AEGeometry **)P(P(self, 0x1c), 4));
    float scale = *(float *)scaleBytes + (float)amount * -9.769497830779909e32f;
    AEGeometry_setScaling3(*(AEGeometry **)P(P(self, 0x1c), 4), scale, scale, scale);
}
