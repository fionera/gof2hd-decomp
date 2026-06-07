#include "class.h"

using AbyssEngine::AEMath::Vector;

struct PlayerGasCloud {
    void translate(Vector const &param_1);
};

extern "C" void AEGeometry_translate_v(void *geom, Vector const &v);

void PlayerGasCloud::translate(Vector const &param_1)
{
    return AEGeometry_translate_v(F<void *>(this, 0x8), param_1);
}
