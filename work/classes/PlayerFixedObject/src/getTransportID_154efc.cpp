#include "class.h"

extern "C" int PlayerFixedObject_getTransportID(PlayerFixedObject *self)
{
    return F<int>(self, 0x1a8);
}
