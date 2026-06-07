#include "class.h"

extern "C" unsigned char SpaceLounge_hangarNeedsUpdate(SpaceLounge *self)
{
    return UC(self, 0x35);
}
