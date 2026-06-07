#include "class.h"

struct AEGeometry {
    void setPosition();
};

void PlayerJumpgate::setPosition(float x, float y, float z)
{
    F<int32_t>(this, 0x124) = (int32_t)x;
    F<int32_t>(this, 0x128) = (int32_t)y;
    F<int32_t>(this, 0x12c) = (int32_t)z;
    F<AEGeometry *>(this, 0x8)->setPosition();
}
