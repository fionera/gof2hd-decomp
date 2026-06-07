#include "class.h"

// Singleton pointer dereferenced twice (PaintCanvas-like). Hidden -> single pc-rel load.
__attribute__((visibility("hidden"))) extern int **g_TB_canvas;

extern "C" bool TouchButton_touchedInside(TouchButton *self, int px, int py)
{
    int x = I(self, 0x78);
    int xm1 = x - 1;
    int h;
    int top;
    if (I(self, 0x70) == 3) {
        int v = (*g_TB_canvas)[0x38 / 4];
        if (xm1 + v > px)
            return false;
        if (I(self, 0x90) + ((x + 1) - v) <= px)
            return false;
        top = I(self, 0x7c);
        bool r = false;
        if (top - 1 > py)
            return r;
        h = I(self, 0x8c);
    } else {
        int m = I(self, 0xc0);
        if (xm1 - m > px)
            return false;
        if (I(self, 0x90) + (x + m) + 1 <= px)
            return false;
        top = I(self, 0x7c) + ~m;
        bool r = false;
        if (top > py)
            return r;
        h = I(self, 0x8c);
        top = I(self, 0x7c) + m;
    }
    return h + top + 1 >= py;
}
