#include "class.h"

// SpacePoint::SpacePoint(int, const Vector&, const Vector&, int)
// Zeroing the 0x18-byte head lowers to a NEON vst1 (16 bytes, post-indexed by 0x14)
// plus a scalar tail store, exactly matching the target. Telling the compiler the
// destination is word-aligned makes that NEON store use 32-bit (.32) elements.
extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);
extern "C" void *memset(void *, int, unsigned long);

struct SpacePoint {
    SpacePoint(int param_1, const Vector &param_2, const Vector &param_3, int param_4);
};

SpacePoint::SpacePoint(int param_1, const Vector &param_2, const Vector &param_3, int param_4)
{
    memset(__builtin_assume_aligned(this, 4), 0, 0x18);
    F<int>(this, 0x18) = param_1;
    Vector_assign((Vector *)this, &param_2);
    Vector_assign((Vector *)((char *)this + 0xc), &param_3);
    F<uint8_t>(this, 0x1c) = 1;
    F<int>(this, 0x20) = param_4;
}
