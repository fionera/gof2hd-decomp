#include "class.h"

extern "C" void Hud_setTimeExtender(Hud *self, bool p1, bool p2, bool p3, bool p4)
{
    UC(self, 0) = p1;
    UC(self, 0x280) = p3;
    UC(self, 0x281) = p4;
    if (p2 && p3) {
        I(self, 0x4c0) = 0x7d0;
        I(self, 0x4bc) = 0x50;
    }
}
