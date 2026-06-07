#include "class.h"

// Globals::resetHints() zeroes four 16-byte blocks of the hint struct (at offsets 0, 0x2b,
// 0x20, 0x10) using a zeroed NEON quad. Offset 0x2b is unaligned, so use packed 16-byte
// vector stores.
typedef int v4si __attribute__((vector_size(16)));
struct __attribute__((packed)) Q16 { v4si v; };

extern void *const gHints __attribute__((visibility("hidden")));

extern "C" void Globals_resetHints()
{
    char *p = (char *)gHints;
    const v4si z = {0, 0, 0, 0};
    ((Q16 *)(p + 0x00))->v = z;
    ((Q16 *)(p + 0x2b))->v = z;
    ((Q16 *)(p + 0x20))->v = z;
    ((Q16 *)(p + 0x10))->v = z;
}
