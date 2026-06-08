#include "class.h"

extern "C" void SpacePoint_giveFree(SpacePoint *self)
{
    F<uint8_t>(self, 0x1c) = 1;
}
