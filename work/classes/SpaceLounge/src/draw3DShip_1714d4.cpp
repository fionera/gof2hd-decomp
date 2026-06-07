#include "class.h"

extern "C" void SpaceLounge_draw3DShip_tail(void *ship);

extern "C" void SpaceLounge_draw3DShip(SpaceLounge *self)
{
    if (UC(self, 0x1c) != 0) {
        return SpaceLounge_draw3DShip_tail(P(self, 0xc));
    }
}
