#include "class.h"

extern "C" uint8_t Hud_jumpMapSelected(Hud *self)
{
    return UC(self, 0x274);
}
