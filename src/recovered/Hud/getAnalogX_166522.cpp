#include "class.h"

extern "C" float Hud_getAnalogX(Hud *self)
{
    float num = (float)((int)US(self, 0x41e) - (int)US(self, 0x424));
    float den = (float)I(self, 0x4e0);
    return num / den;
}
