#include "class.h"

extern "C" Vector StarSystem_getLightDirection(StarSystem *self)
{
    return F<Vector>(self, 0x30);
}
