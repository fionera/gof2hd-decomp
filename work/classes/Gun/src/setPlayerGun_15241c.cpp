#include "class.h"

extern "C" void Gun_setPlayerGun(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf0) = v;
}
