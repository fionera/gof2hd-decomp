#include "class.h"

extern "C" uint8_t PlayerFighter_hasCrateCaptured(PlayerFighter *self)
{
    return F<uint8_t>(self, 0x4c) == 0;
}
