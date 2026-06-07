#include "class.h"

void PlayerAsteroid::setRotationEnabled(bool enabled)
{
    F<uint8_t>(this, 0x14c) = enabled;
}
