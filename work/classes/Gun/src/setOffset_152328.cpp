#include "class.h"

// NEAR: same family as the Vector setOffset — the target carries a stack canary for the
// on-stack Vector buffer. Under basic -fstack-protector (not -strong) clang emits no
// canary for a Vector-typed buffer at -Oz, so the prologue/epilogue diverge. Logic below
// matches the decompile (table lookup of a packed 3*int16 offset, added to the gun's
// offset Vector, then Vector::operator=).

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

extern "C" void Vector_assign(Vector *dst, const Vector *src);   // 0x6eb3c

// pc-rel globals.
extern int *const gSO_holder __attribute__((visibility("hidden")));
extern short *const gSO_table __attribute__((visibility("hidden")));

extern "C" void Gun_setOffset_ii(Gun *self, int a, int b)
{
    short *row = (short *)((char *)gSO_table + b * 0x3c + a * 6);
    Vector local;
    local.x = (float)(int)row[0];
    local.y = (float)(int)row[1];
    local.z = (float)(int)row[2];
    local.x = F<float>(self, 0x7c) + local.x;
    local.y = F<float>(self, 0x80) + local.y;
    local.z = F<float>(self, 0x84) + local.z;
    Vector_assign((Vector *)((char *)self + 0x7c), &local);
}
