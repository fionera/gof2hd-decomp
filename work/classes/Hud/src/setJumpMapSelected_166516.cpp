#include "class.h"

extern "C" void Hud_setJumpMapSelected(Hud *self, bool value)
{
    UC(self, 0x274) = value;
}
