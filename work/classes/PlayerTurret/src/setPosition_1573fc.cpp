#include "class.h"

using AbyssEngine::AEMath::Vector;

extern "C" void AEGeometry_setPosition(AEGeometry *self, const Vector *position);

struct PlayerTurret {
    void setPosition(const Vector &position);
};

void PlayerTurret::setPosition(const Vector &position)
{
    AEGeometry_setPosition(TP<AEGeometry>(this, 0x8), &position);
    I(this, 0x58) = *(const int *)B(&position, 0x0);
    I(this, 0x5c) = *(const int *)B(&position, 0x4);
    I(this, 0x60) = *(const int *)B(&position, 0x8);
}
