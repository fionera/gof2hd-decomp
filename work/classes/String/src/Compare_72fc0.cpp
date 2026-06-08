#include "class.h"

// AbyssEngine::String::Compare(char const*) - compare against an 8-bit string.
extern "C" unsigned int String_Compare_char(String *self, const char *s)
{
    if (F<int>(self, 0x8) == 0)
        return 1;

    bool reachedEnd = false;
    uint16_t cur;
    unsigned int other;
    uint16_t *p = F<uint16_t *>(self, 0x4);
    for (; (cur = *p) != 0; p++) {
        other = (unsigned int)(unsigned char)*s;
        if (other == 0 || other != cur)
            goto done;
        s++;
    }
    other = (unsigned int)(unsigned char)*s;
    reachedEnd = true;
    cur = 0;
done:
    if (other != 0) {
        char d = (char)cur - (char)other;
        if (reachedEnd)
            d = -1;
        return (unsigned int)(int)d;
    }
    return (unsigned int)(cur != 0);
}
