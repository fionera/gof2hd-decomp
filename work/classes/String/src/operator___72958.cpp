#include "class.h"

extern "C" {
String *String_ctor_float(String *self, float v);
void String_dtor(String *self);
void *realloc(void *p, unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
}

struct SBuf { uint32_t a, b, c; };

// AbyssEngine::String::operator+=(float const&) - append the formatted form of a float.
extern "C" String *String_addAssign_float(String *self, const float *v)
{
    SBuf tmp;
    String_ctor_float((String *)&tmp, *v);
    unsigned int addLen = *(uint32_t *)((char *)&tmp + 0x8);
    if (addLen != 0) {
        void *addData = *(void **)((char *)&tmp + 0x4);
        int oldLen = F<int>(self, 0x8);
        int newLen = addLen + oldLen;
        F<int>(self, 0x8) = newLen;
        void *buf = realloc(F<void *>(self, 0x4), newLen * 2 + 2);
        F<void *>(self, 0x4) = buf;
        __aeabi_memcpy((char *)buf + oldLen * 2, addData, addLen << 1);
    }
    String_dtor((String *)&tmp);
    return self;
}
