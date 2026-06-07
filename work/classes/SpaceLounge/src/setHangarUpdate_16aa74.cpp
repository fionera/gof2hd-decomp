#include "class.h"

extern "C" void SpaceLounge_setHangarUpdate(SpaceLounge *self, bool value)
{
    BL(self, 0x35) = value;
}
