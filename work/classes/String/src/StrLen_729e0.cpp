#include "class.h"

// AbyssEngine::String::StrLen(char const*) - byte length of a NUL-terminated char string.
extern "C" int String_StrLen_char(String *self, const char *s)
{
    const char *p = s;
    char c;
    do {
        c = *p;
        p++;
    } while (c != '\0');
    return (int)(p + ~(unsigned int)s - (char *)0);
}
