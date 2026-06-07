#include "class.h"

extern "C" void Gun_setFriendGun(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf9) = v;
}
