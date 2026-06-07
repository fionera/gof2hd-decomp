#include "class.h"

extern "C" uint8_t Hud_isHackingGameActive(Hud *self)
{
    return UC(self, 0x528);
}
