#include "class.h"

extern "C" void SpacePoint_take(SpacePoint *self)
{
    F<uint8_t>(self, 0x1c) = 0;
}
