#include "class.h"

extern "C" bool HangarWindow_listMode(HangarWindow *self)
{
    if (F<int>(self, 0x58) == 0 && F<uint8_t>(self, 0x89) == 0) {
        return F<uint8_t>(self, 0x3c) == 0;
    }
    return false;
}
