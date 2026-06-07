#include "class.h"

extern "C" void String_ctor_default(void *self);
extern "C" void *operator_new(unsigned int size);
extern "C" void ScrollTouchBox_ctor(void *self, int x, int y, int w, int h);

__attribute__((visibility("hidden"))) extern void **g_STW_layout_1741c0;

ScrollTouchWindow::ScrollTouchWindow(int x, int y, int w, int h)
{
    String_ctor_default(B(this, 4));
    I(this, 0x14) = x;
    I(this, 0x18) = y;
    I(this, 0x1c) = w;
    I(this, 0x20) = h;

    void *box = operator_new(0x40);
    void *layout = *g_STW_layout_1741c0;
    int top = I(layout, 8);
    int border = I(layout, 0x4c);
    ScrollTouchBox_ctor(box, border + x, top + border + y, w - border * 2,
                        (h - top) - border * 2);
    *(void *volatile *)B(this, 0) = box;
    *(unsigned short volatile *)B(this, 0x10) = 0x100;
}
