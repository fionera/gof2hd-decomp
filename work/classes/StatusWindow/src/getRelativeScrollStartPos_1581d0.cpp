#include "class.h"

// StatusWindow::getRelativeScrollStartPos() -> 0 if scroll range (this+0x38) > 0,
// else -(float)range / (float)(this+0x58).
extern "C" float StatusWindow_getRelativeScrollStartPos(StatusWindow *self)
{
    int range = i32(self, 0x38);
    if (range > 0) return 0.0f;
    return -(float)range / (float)i32(self, 0x58);
}
