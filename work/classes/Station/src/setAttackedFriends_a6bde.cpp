#include "class.h"

extern "C" void Station_setAttackedFriends(Station *self, bool v)
{
    self->f_24 = v;
}
