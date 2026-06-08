#include "class.h"

extern "C" void ScrollTouchBox_setTextColor(void *self, AbyssEngine::String *text, int color);

void ScrollTouchWindow::setText(AbyssEngine::String title, AbyssEngine::String text, int color)
{
    {
        void *box = this->f_0;
        AbyssEngine::String tmp(text, false);
        ScrollTouchBox_setTextColor(box, &tmp, color);
    }
    *(AbyssEngine::String *)B(this, 4) = title;
}
