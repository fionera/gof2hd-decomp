#include "class.h"

extern "C" void ScrollTouchBox_setText(void *self, AbyssEngine::String *text);

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text)
{
    {
        void *box = this->f_0;
        AbyssEngine::String tmp(text, false);
        ScrollTouchBox_setText(box, &tmp);
    }
    *(AbyssEngine::String *)B(this, 4) = title;
}
