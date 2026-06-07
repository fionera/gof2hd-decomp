#include "class.h"

extern "C" void *ScrollTouchBox_dtor(void *self);
extern "C" void operator_delete(void *self);
extern "C" void String_dtor(void *self);

ScrollTouchWindow::~ScrollTouchWindow()
{
    void *box = P(this, 0);
    if (box != 0) {
        operator_delete(ScrollTouchBox_dtor(box));
    }

    int *p = (int *)this;
    *p++ = 0;
    String_dtor(p);
}
