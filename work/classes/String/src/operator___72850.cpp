#include "class.h"

extern "C" {
void *operator_new__(unsigned int n);
void *realloc(void *p, unsigned int n);
}

// AbyssEngine::String::operator+=(char const&) - append a single character.
extern "C" String *String_addAssign_char(String *self, const char *c)
{
    int len = F<int>(self, 0x8);
    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(4);
        F<void *>(self, 0x4) = buf;
        unsigned char ch = *(const unsigned char *)c;
        buf[1] = 0;
        buf[0] = (uint16_t)ch;
        F<uint32_t>(self, 0x8) = 1;
    } else {
        F<int>(self, 0x8) = len + 1;
        char *buf = (char *)realloc(F<void *>(self, 0x4), len * 2 + 4);
        F<void *>(self, 0x4) = buf;
        int nlen = F<int>(self, 0x8);
        unsigned char ch = *(const unsigned char *)c;
        *(uint16_t *)(buf + nlen * 2) = 0;
        *(uint16_t *)(buf + nlen * 2 - 2) = (uint16_t)ch;
    }
    return self;
}
