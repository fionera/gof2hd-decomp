#include "class.h"

extern "C" void PlayerFixedObject_setDockingType(PlayerFixedObject *self, int v)
{
    F<int>(self, 0x1a4) = v;
}
