#include "class.h"

extern "C" void *TouchButton_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void String_dtor(void *self);
void operator delete(void *ptr) noexcept;

ChoiceWindow::~ChoiceWindow()
{
    void *button = F<void *>(this, 0x10);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x10) = 0;

    button = F<void *>(this, 0x14);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x14) = 0;

    button = F<void *>(this, 0x18);
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    F<void *>(this, 0x18) = 0;

    void *scroll = F<void *>(this, 0x1c);
    if (scroll != 0)
        operator delete(ScrollTouchWindow_dtor(scroll));
    void *title = (char *)this + 0x3c;
    F<void *>(this, 0x1c) = 0;
    String_dtor(title);
    String_dtor((char *)this + 0x20);
}
