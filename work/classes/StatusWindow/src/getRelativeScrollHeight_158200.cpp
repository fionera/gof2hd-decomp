#include "class.h"

// StatusWindow::getRelativeScrollHeight() -> ratio of visible content to total, clamped by range.
extern "C" float StatusWindow_getRelativeScrollHeight(StatusWindow *self)
{
    int a = self->f_58;
    int b = self->f_5c;
    if (b > a) return 0.0f;
    int range = self->f_38;
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
