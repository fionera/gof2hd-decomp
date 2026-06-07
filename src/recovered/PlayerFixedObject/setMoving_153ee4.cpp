#include "class.h"

extern "C" void PlayerFixedObject_setMoving(PlayerFixedObject *self, bool v)
{
    F<uint8_t>(self, 0x134) = v;
}
