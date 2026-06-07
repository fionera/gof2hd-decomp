#include "class.h"

extern "C" bool SpaceLounge_listMode(SpaceLounge *self)
{
    return UC(self, 0x34) == 0;
}
