#include "class.h"

// AbyssEngine::String::ReplaceChar(char from, char to) - replace every matching code unit.
extern "C" void String_ReplaceChar(String *self, char from, char to)
{
    uint16_t *p = F<uint16_t *>(self, 0x4);
    if (p == 0)
        return;
    int count = F<int>(self, 0x8);
    while (count != 0) {
        count = count - 1;
        if ((unsigned int)*p == (unsigned int)(int)from)
            *p = (uint16_t)(short)to;
        p++;
    }
}
