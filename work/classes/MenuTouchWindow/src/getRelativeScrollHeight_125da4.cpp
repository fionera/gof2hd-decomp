#include "class.h"

// MenuTouchWindow::getRelativeScrollHeight()
//   Returns the visible fraction of the scrollable list (page / content), clamped by the
//   current scroll offset at +0x194. Content height at +0x228, page height at +0x22c.
struct MenuTouchWindow { float getRelativeScrollHeight(); };
float MenuTouchWindow::getRelativeScrollHeight()
{
    int content = i32(this, 0x228);
    int page = i32(this, 0x22c);
    if (page < content) {
        union { unsigned u; float f; } c;
        c.u = 0x3f800000u; // 1.0f (DAT_00135df4)
        return c.f;
    }
    int off = i32(this, 0x194);
    int numer;
    if (off >= 1) {
        numer = content - off;
    } else {
        if (content - page <= off) {
            return (float)page / (float)page;
        }
        numer = off + page;
    }
    return (float)numer / (float)page;
}
