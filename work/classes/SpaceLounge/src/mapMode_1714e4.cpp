#include "class.h"

extern "C" unsigned char SpaceLounge_mapMode(SpaceLounge *self)
{
    return UC(self, 0x34);
}
