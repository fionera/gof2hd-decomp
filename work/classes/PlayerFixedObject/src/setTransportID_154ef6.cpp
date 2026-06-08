#include "class.h"

extern "C" void PlayerFixedObject_setTransportID(PlayerFixedObject *self, int v)
{
    F<int>(self, 0x1a8) = v;
}
