#include "class.h"

extern "C" void Hud_resetAnalogStick(Hud *self)
{
    I(self, 0x41e) = I(self, 0x424);
}
