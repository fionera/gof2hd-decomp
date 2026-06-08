#include "class.h"

// Returned for out-of-range indices: a PC-relative pointer to a static NUL code unit.
__attribute__((visibility("hidden"))) extern uint16_t g_String_nullChar;

// AbyssEngine::String::operator[](int) const - pointer to the index-th code unit, or &NUL on OOB.
extern "C" uint16_t *String_index_const(String *self, int i)
{
    uint16_t *fallback = &g_String_nullChar;
    if (i < 0)
        return fallback;
    if (i < F<int>(self, 0x8))
        return (uint16_t *)((char *)F<void *>(self, 0x4) + i * 2);
    return fallback;
}
