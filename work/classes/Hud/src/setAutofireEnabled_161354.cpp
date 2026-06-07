#include "class.h"

extern "C" void Hud_setAutofireEnabled(Hud *self, bool value)
{
    UC(self, 0x4a0) = value;
}
