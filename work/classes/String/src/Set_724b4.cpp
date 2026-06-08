#include "class.h"

extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
}

// Empty-string literal substituted when a null pointer is passed.
static const char kEmpty[] = "";

// AbyssEngine::String::Set(char const*) - replace contents from an 8-bit string (widened to 16-bit).
extern "C" void String_Set_char(String *self, const char *s)
{
    void *old = F<void *>(self, 0x4);
    if (old != 0)
        operator_delete__(old);
    F<void *>(self, 0x4) = 0;

    // Null input falls back to the empty literal (matching the target's retry loop).
    if (s == 0)
        s = kEmpty;

    const unsigned char *in = (const unsigned char *)s;
    const unsigned char *p = in;
    unsigned char c;
    do {
        c = *p;
        p++;
    } while (c != 0);
    unsigned int len = (unsigned int)(p - in) - 1;   // code units excluding terminator
    F<uint32_t>(self, 0x8) = len;

    if (len == 0) {
        uint16_t *buf = (uint16_t *)operator_new__(2);
        F<void *>(self, 0x4) = buf;
        buf[0] = 0;
        return;
    }

    if (in[len - 1] == 0) {
        // Last byte already NUL: copy len units, terminate at len-1.
        unsigned int bytes = len * 2;
        if (len + len < len)
            bytes = 0xffffffff;
        uint16_t *buf = (uint16_t *)operator_new__(bytes);
        F<void *>(self, 0x4) = buf;
        for (unsigned int i = 0; i < len; i++)
            buf[i] = (uint16_t)in[i];
        F<uint32_t>(self, 0x8) = len - 1;
    } else {
        unsigned int count = len + 1;
        unsigned int bytes = count * 2;
        if (count + count < count)
            bytes = 0xffffffff;
        uint16_t *buf = (uint16_t *)operator_new__(bytes);
        F<void *>(self, 0x4) = buf;
        for (unsigned int i = 0; i < len; i++)
            buf[i] = (uint16_t)in[i];
        buf[len] = 0;
    }
}
