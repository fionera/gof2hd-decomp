#include "class.h"

// Returns float in r0. If scroll range (this+0xb4) >= 1 returns a constant; otherwise
// returns -(float)range / (float)contentHeight(this+0xd4).
extern "C" float HangarWindow_getRelativeScrollStartPos(HangarWindow *self)
{
    int range = F<int>(self, 0xb4);
    if (range > 0) {
        union { unsigned u; float f; } c; c.u = 0x4650a903u;
        return c.f;
    }
    return -(float)range / (float)F<int>(self, 0xd4);
}
