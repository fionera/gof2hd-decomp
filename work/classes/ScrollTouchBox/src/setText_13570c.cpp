#include "class.h"

extern "C" void String_ctor_string(void *self, String *src, bool copy);
extern "C" void String_dtor(void *self);
extern "C" void ScrollTouchBox_setText2(ScrollTouchBox *self, String *text, int font);

__attribute__((visibility("hidden"))) extern void **g_ScrollTouchBox_defaultWidth_13570c;

void ScrollTouchBox::setText(AbyssEngine::String text)
{
    char tmp[12];

    String_ctor_string(tmp, &text, false);
    ScrollTouchBox_setText2(this, (String *)tmp, **(int **)g_ScrollTouchBox_defaultWidth_13570c);
    String_dtor(&tmp);
}
