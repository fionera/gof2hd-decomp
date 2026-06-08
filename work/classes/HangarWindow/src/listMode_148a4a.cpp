#include "class.h"

extern "C" bool HangarWindow_listMode(HangarWindow *self)
{
    if (self->f_58 == 0 && F<uint8_t>(self, 0x89) == 0) {
        return self->f_3c == 0;
    }
    return false;
}
