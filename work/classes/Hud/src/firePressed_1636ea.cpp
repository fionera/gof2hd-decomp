#include "class.h"

extern "C" unsigned int Hud_firePressed(Hud *self)
{
    return (UC(self, 0x284) >> 4) & 1;
}
