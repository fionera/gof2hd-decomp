#include "class.h"

// AbyssEngine::String::StrLen(unsigned short const*) - length of a NUL-terminated wide string.
extern "C" int String_StrLen_wchar(String *self, const uint16_t *s)
{
    if (s == 0)
        return 0;
    const uint16_t *p = s;
    uint16_t c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    return (int)((p - s) - 1);
}
