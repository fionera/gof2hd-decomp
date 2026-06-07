#include "class.h"

extern "C" void PlayerFighter_setAIDisabled(PlayerFighter *self, bool v)
{
    F<uint8_t>(self, 0x2e4) = v;
}
