#include "class.h"

extern "C" void PlayerFighter_setShootError(PlayerFighter *self, int v)
{
    F<float>(self, 0x1a8) = (float)v;
}
