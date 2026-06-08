#include "class.h"

extern "C" void *operator_new__(unsigned int n);

// AbyssEngine::String::GetAEChar() const - allocate an 8-bit (low-byte) copy of the wide buffer.
extern "C" char *String_GetAEChar(String *self)
{
    unsigned int len = F<uint32_t>(self, 0x8);
    if (len != 0) {
        unsigned int n = len + 1;
        char *out = (char *)operator_new__(n);
        for (unsigned int i = 0; i != n; i++)
            out[i] = *(char *)((char *)F<void *>(self, 0x4) + i * 2);
        return out;
    }
    char *out = (char *)operator_new__(1);
    out[0] = 0;
    return out;
}
