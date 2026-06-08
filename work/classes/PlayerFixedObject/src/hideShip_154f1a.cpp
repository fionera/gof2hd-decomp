#include "class.h"

extern "C" void PlayerFixedObject_hideShip(PlayerFixedObject *self)
{
    F<uint8_t>(self, 0x1b8) = 1;
}
