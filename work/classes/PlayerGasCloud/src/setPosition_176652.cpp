#include "class.h"

using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud {
    void setPosition(Vector const &param_1);
};

extern "C" void AEGeometry_setPosition_v(void *geom, Vector const &v);

void PlayerGasCloud::setPosition(Vector const &param_1)
{
    return AEGeometry_setPosition_v(F<void *>(this, 0x8), param_1);
}
