#include "class.h"

extern "C" void PlayerFighter_setRotate(PlayerFighter *self, int v)
{
    float f = (float)v;
    F<uint8_t>(self, 0x13d) = 0;
    F<float>(self, 0x1a4) = f;
    F<float>(self, 0x1f0) = f;
}
