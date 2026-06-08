#include "class.h"

extern "C" {
void operator_delete__(void *p);
void *operator_new__(unsigned int n);
unsigned long long __aeabi_uldivmod(unsigned long long num, unsigned long long den);
void String_Set_char(String *self, const char *s);   // virtual Set(char const*) fallback
}

// AbyssEngine::String::Set(long long) - format a signed 64-bit integer as a decimal string.
extern "C" void String_Set_longlong(String *self, long long v)
{
    void *old = F<void *>(self, 0x4);
    if (old != 0)
        operator_delete__(old);
    F<void *>(self, 0x4) = 0;

    if (v == 0) {
        String_Set_char(self, "");
        return;
    }

    uint16_t *buf = (uint16_t *)operator_new__(0x28);
    F<void *>(self, 0x4) = buf;
    F<uint32_t>(self, 0x8) = 0;

    unsigned long long mag;
    uint16_t *digits;       // where the first generated digit is written
    unsigned int count;     // running digit count (1 if a sign was emitted)
    if (v < 0) {
        buf[0] = '-';
        digits = buf + 1;
        mag = (unsigned long long)(-v);
        count = 1;
        F<uint32_t>(self, 0x8) = 1;
    } else {
        digits = buf;
        mag = (unsigned long long)v;
        count = 0;
    }

    // Emit decimal digits least-significant first, just past the sign slot.
    uint16_t *w = digits;
    do {
        unsigned long long q = __aeabi_uldivmod(mag, 10);
        unsigned int rem = (unsigned int)(mag - q * 10);
        unsigned char ch = (unsigned char)rem | 0x30;
        if (rem > 9)
            ch = (unsigned char)rem + 0x57;
        *w = ch;
        w++;
        count++;
        mag = q;
    } while (mag != 0);
    F<uint32_t>(self, 0x8) = count;
    *w = 0;

    // Reverse the generated digit run in place so it reads most-significant first.
    uint16_t *lo = digits;
    uint16_t *hi = w - 1;
    while (lo < hi) {
        uint16_t t = *lo;
        *lo = *hi;
        *hi = t;
        lo++;
        hi--;
    }
}
