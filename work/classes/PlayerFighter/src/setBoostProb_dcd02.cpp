#include "class.h"

extern "C" void PlayerFighter_setBoostProb(PlayerFighter *self, int v)
{
    F<int32_t>(self, 0x1b4) = v;
}
