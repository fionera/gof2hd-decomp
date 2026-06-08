#include "class.h"

// StatusWindow::getRelativeScrollHeight() -> ratio of visible content to total, clamped by range.
extern "C" float StatusWindow_getRelativeScrollHeight(StatusWindow *self)
{
    int a = i32(self, 0x58);
    int b = i32(self, 0x5c);
    if (b > a) return 0.0f;
    int range = i32(self, 0x38);
    int num;
    if (range >= 1) {
        num = b - range;
    } else if (range < b - a) {
        num = range + a;
    } else {
        num = b;
    }
    return (float)num / (float)a;
}
