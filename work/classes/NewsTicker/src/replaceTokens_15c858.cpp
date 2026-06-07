#include "class.h"

extern "C" void String_copy_ctor(void *self, void *other, bool copy);

String NewsTicker::replaceTokens(String text)
{
    String out;
    String_copy_ctor(&out, &text, false);
    return out;
}
