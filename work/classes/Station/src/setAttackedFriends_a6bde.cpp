#include "class.h"

extern "C" void Station_setAttackedFriends(Station *self, bool v)
{
    F<uint8_t>(self, 0x24) = v;
}
