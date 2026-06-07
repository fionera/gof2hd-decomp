#include "class.h"

extern "C" uint8_t PlayerFighter_hasMissionCrateLost(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x68);
}
