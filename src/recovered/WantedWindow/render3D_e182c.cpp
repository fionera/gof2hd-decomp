#include "class.h"

extern "C" void WantedWindow_render3D_tail(void *starMap);

extern "C" void WantedWindow_render3D(WantedWindow *self)
{
    if (F<uint8_t>(self, 0x14) != 0) {
        return WantedWindow_render3D_tail(F<void *>(self, 0x4));
    }
}
