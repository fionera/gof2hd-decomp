#include "class.h"

extern "C" float WantedWindow_getRelativeScrollStartPos(WantedWindow *self)
{
    int pos = F<int>(self, 0x84);
    if (pos > 0) {
        union { uint32_t u; float f; } c;
        c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)pos / (float)F<int>(self, 0xa4);
}
