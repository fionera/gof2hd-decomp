#include "class.h"

extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
void *__aeabi_memcpy(void *dst, const void *src, unsigned int n);
void String_Set_char(String *self, const char *s);   // virtual Set(char const*) fallback
}

// AbyssEngine::String::Set(unsigned short const*) - replace contents from a wide string.
extern "C" void String_Set_wchar(String *self, const uint16_t *s)
{
    void *old = F<void *>(self, 0x4);
    if (old != 0)
        operator_delete__(old);
    F<void *>(self, 0x4) = 0;

    if (s == 0) {
        // Null -> empty string via the char-Set path (tail call in the target).
        String_Set_char(self, "");
        return;
    }

    const uint16_t *p = s;
    uint16_t c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    unsigned int count = (unsigned int)(p - s);     // includes the terminator
    unsigned int len = count - 1;
    F<uint32_t>(self, 0x8) = len;

    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(2);
        F<void *>(self, 0x4) = buf;
        buf[0] = 0;
        return;
    }

    unsigned int last = count - 2;
    if (s[last] == 0) {
        unsigned int bytes = len * 2;
        if (len + len < len)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        F<void *>(self, 0x4) = buf;
        __aeabi_memcpy(buf, s, len * 2);
        *(uint16_t *)((char *)buf + last * 2) = 0;
        F<uint32_t>(self, 0x8) = last;
    } else {
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        void *buf = operator_new__(bytes);
        F<void *>(self, 0x4) = buf;
        __aeabi_memcpy(buf, s, len * 2);
        *(uint16_t *)((char *)buf + len * 2) = 0;
    }
}
