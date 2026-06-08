#include "class.h"

extern "C" int PlayerFixedObject_getDockingType(PlayerFixedObject *self)
{
    return F<int>(self, 0x1a4);
}
