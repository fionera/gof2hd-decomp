#include "class.h"

extern "C" void HangarWindow_hideMessage(HangarWindow *self)
{
    F<uint8_t>(self, 0x3c) = 0;
}
