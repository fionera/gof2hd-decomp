#include "class.h"

extern "C" void AEGeometry_setScaling(AEGeometry *self, float x, float y, float z);

struct PlayerTurret {
    void setScaling(float scale);
};

void PlayerTurret::setScaling(float scale)
{
    AEGeometry_setScaling(TP<AEGeometry>(this, 0x148), scale, scale, scale);
}
