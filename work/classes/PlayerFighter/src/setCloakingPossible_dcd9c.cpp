#include "class.h"

extern "C" void PlayerFighter_cloak_off_helper();

extern "C" void PlayerFighter_setCloakingPossible(PlayerFighter *self, bool v)
{
    F<uint8_t>(self, 0x2d8) = v;
    if (!v && F<uint8_t>(self, 0x13c) != 0) {
        F<int32_t>(self, 0x2c8) = F<int32_t>(self, 0x2cc) + 1;
        return PlayerFighter_cloak_off_helper();
    }
}
