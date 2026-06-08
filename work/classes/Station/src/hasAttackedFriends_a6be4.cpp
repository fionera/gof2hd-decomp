#include "class.h"

extern "C" uint8_t Station_hasAttackedFriends(Station *self)
{
    return self->f_24;
}
