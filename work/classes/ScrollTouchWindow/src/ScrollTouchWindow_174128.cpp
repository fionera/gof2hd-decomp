#include "class.h"

extern "C" void String_ctor_default(void *self);
extern "C" void *operator_new(unsigned int size);
extern "C" void ScrollTouchBox_ctor(void *self, int x, int y, int w, int h);

__attribute__((visibility("hidden"))) extern void **g_STW_layout_174128;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h, bool hasFrame)
{
    String_ctor_default(B(this, 4));
    int *geom = (int *)B(this, 0x14);
    geom[0] = x;
    geom[1] = y;
    geom[2] = w;
    I(this, 0x20) = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_174128;
    int border = I(layout, 0x4c);
    int extra;
    int boxY;
    if (hasFrame) {
        int top = I(layout, 8);
        boxY = border + y + top;
        extra = -top;
    } else {
        boxY = border + y;
        extra = 0;
    }
    ScrollTouchBox_ctor(box, border + x, boxY, w - border * 2,
                        extra + h - border * 2);
    *(unsigned char volatile *)B(this, 0x10) = 0;
    *(void *volatile *)B(this, 0) = box;
    *(unsigned char volatile *)B(this, 0x11) = hasFrame;
}
