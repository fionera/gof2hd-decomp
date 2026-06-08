#include "class.h"

extern "C" void *TouchButton_dtor(void *self);
extern "C" void *ScrollTouchWindow_dtor(void *self);
extern "C" void String_dtor(void *self);
void operator delete(void *ptr) noexcept;

ChoiceWindow::~ChoiceWindow()
{
    void *button = this->f_10;
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    this->f_10 = 0;

    button = this->f_14;
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    this->f_14 = 0;

    button = this->f_18;
    if (button != 0)
        operator delete(TouchButton_dtor(button));
    this->f_18 = 0;

    void *scroll = this->f_1c;
    if (scroll != 0)
        operator delete(ScrollTouchWindow_dtor(scroll));
    void *title = (char *)this + 0x3c;
    this->f_1c = 0;
    String_dtor(title);
    String_dtor((char *)this + 0x20);
}
