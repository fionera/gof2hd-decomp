#include "class.h"

extern "C" void PlayerFighter_setSpeed(PlayerFighter *self, float v)
{
    F<float>(self, 0x1ac) = v;
    F<uint8_t>(self, 0x13d) = 0;
    F<float>(self, 0x1e8) = v;
}
