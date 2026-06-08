#include "class.h"

extern "C" {
String *String_ctor_copy(String *self, String *other, bool rev);
void String_dtor(String *self);
void *realloc(void *p, unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::operator+=(AbyssEngine::String const&)
extern "C" String *String_addAssign_str(String *self, String *other)
{
    if (F<int>(other, 0x8) != 0) {
        SBuf tmp;
        String_ctor_copy((String *)&tmp, other, false);
        unsigned int addLen = *(uint32_t *)((char *)&tmp + 0x8);
        void *addData = *(void **)((char *)&tmp + 0x4);

        int oldLen = F<int>(self, 0x8);
        int newLen = addLen + oldLen;
        F<int>(self, 0x8) = newLen;
        void *buf = realloc(F<void *>(self, 0x4), newLen * 2 + 2);
        F<void *>(self, 0x4) = buf;
        __aeabi_memcpy((char *)buf + oldLen * 2, addData, addLen << 1);
        *(uint16_t *)((char *)buf + F<int>(self, 0x8) * 2) = 0;
        String_dtor((String *)&tmp);
    }
    return self;
}
