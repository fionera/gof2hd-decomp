#include "class.h"

using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud {
    Vector getPosition();
};

extern "C" Vector AEGeometry_getPosition_ret(void *geom);

Vector PlayerGasCloud::getPosition()
{
    return AEGeometry_getPosition_ret(F<void *>(this, 0x8));
}
