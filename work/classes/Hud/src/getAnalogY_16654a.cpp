#include "class.h"

extern "C" float Hud_getAnalogY(Hud *self)
{
    float num = (float)((int)US(self, 0x420) - (int)US(self, 0x426));
    float den = (float)I(self, 0x4e0);
    return num / den;
}
