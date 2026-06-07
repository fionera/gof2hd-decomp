#include "class.h"

extern "C" void Hud_setHackingGameActive(Hud *self, bool value)
{
    UC(self, 0x528) = value;
}
