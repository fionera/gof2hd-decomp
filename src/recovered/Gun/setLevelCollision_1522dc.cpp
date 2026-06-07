#include "class.h"

extern "C" void Gun_setLevelCollision(Gun *self, bool v)
{
    F<uint8_t>(self, 0xf8) = v;
}
