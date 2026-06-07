#include "class.h"

extern "C" void Hud_playerHit(Hud *self)
{
    UC(self, 0x244) = 1;
    I(self, 0x46c) = 0;
}
