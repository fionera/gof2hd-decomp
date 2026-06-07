#include "class.h"

extern "C" uint8_t Station_hasAttackedFriends(Station *self)
{
    return F<uint8_t>(self, 0x24);
}
