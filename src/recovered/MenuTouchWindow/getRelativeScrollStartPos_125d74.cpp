#include "class.h"

// MenuTouchWindow::getRelativeScrollStartPos()
//   r = CONST; if (this->0x194 <= 0) r = -(float)(int)this->0x194 / (float)(int)this->0x22c;
struct MenuTouchWindow { float getRelativeScrollStartPos(); };
float MenuTouchWindow::getRelativeScrollStartPos()
{
    int a = i32(this, 0x194);
    if (a > 0) {
        union { unsigned u; float f; } c;
        c.u = 0x9a132100u;
        return c.f;
    }
    return -(float)a / (float)i32(this, 0x22c);
}
