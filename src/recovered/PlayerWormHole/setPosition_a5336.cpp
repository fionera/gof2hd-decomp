#include "class.h"

struct PlayerWormHole {
    void setPosition(float x, float y, float z);
};

extern "C" void AEGeometry_positionChanged(void *geometry);

void PlayerWormHole::setPosition(float x, float y, float z)
{
    F<float>(this, 0x58) = x;
    F<float>(this, 0x5c) = y;
    F<float>(this, 0x60) = z;
    F<int>(this, 0x128) = (int)y;
    F<int>(this, 0x124) = (int)x;
    F<int>(this, 0x12c) = (int)z;
    return AEGeometry_positionChanged(F<void *>(this, 0x8));
}
