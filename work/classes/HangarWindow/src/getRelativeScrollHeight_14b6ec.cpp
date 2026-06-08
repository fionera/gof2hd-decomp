#include "class.h"

// Returns float in r0. a=this+0xd4 (denominator), b=this+0xd8.
// NEAR: clang pairs a->r2,b->r1 in the ldrd (target uses r1=a,r2=b), mirroring every
// subsequent op; register pairing is an allocator tie-break, not source-driven.
extern "C" float HangarWindow_getRelativeScrollHeight(HangarWindow *self)
{
    int a = self->f_d4;
    int b = self->f_d8;
    if (a < b) {
        union { unsigned u; float f; } c; c.u = 0x4605e009u;
        return c.f;
    }
    int e = self->f_b4;
    int num;
    if (e >= 1) {
        num = b - e;
    } else if (e >= b - a) {
        num = b;
    } else {
        num = e + a;
    }
    return (float)num / (float)a;
}
