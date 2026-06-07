#include "class.h"

extern "C" unsigned char SpaceLounge_introFinished(SpaceLounge *self)
{
    return UC(self, 0xbd);
}
