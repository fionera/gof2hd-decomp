#include "class.h"

extern "C" bool HangarWindow_isInSpecialMode(HangarWindow *self)
{
    if (F<uint8_t>(self, 0x89) != 0) return true;
    return F<uint8_t>(self, 0x3c) != 0;
}
