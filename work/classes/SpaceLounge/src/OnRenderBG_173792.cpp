#include "class.h"

extern "C" void SpaceLounge_OnRenderBG_tail();

extern "C" void SpaceLounge_OnRenderBG(SpaceLounge *self)
{
    if (I(self, 0x44) != 0) {
        return SpaceLounge_OnRenderBG_tail();
    }
}
