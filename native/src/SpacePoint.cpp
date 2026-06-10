#include "gof2/SpacePoint.h"


extern "C" Vector *Vector_assign(Vector *dst, const Vector *src);
extern "C" void *memset(void *, int, unsigned long);

// ---- giveFree_1603f0.cpp ----
extern "C" void SpacePoint_giveFree(SpacePoint *self)
{
    self->field_0x1c = 1;
}

// ---- SpacePoint_1603ac.cpp ----
// SpacePoint::SpacePoint(int, const Vector&, const Vector&, int)
// Zeroing the 0x18-byte head lowers to a NEON vst1 (16 bytes, post-indexed by 0x14)
// plus a scalar tail store, exactly matching the target. Telling the compiler the
// destination is word-aligned makes that NEON store use 32-bit (.32) elements.

struct SpacePoint {
    SpacePoint(int param_1, const Vector &param_2, const Vector &param_3, int param_4);
};

SpacePoint::SpacePoint(int param_1, const Vector &param_2, const Vector &param_3, int param_4)
{
    memset(__builtin_assume_aligned(this, 4), 0, 0x18);
    this->field_0x18 = param_1;
    Vector_assign((Vector *)this, &param_2);
    Vector_assign((Vector *)((char *)this + 0xc), &param_3);
    this->field_0x1c = 1;
    this->field_0x20 = param_4;
}

// ---- take_1603ea.cpp ----
extern "C" void SpacePoint_take(SpacePoint *self)
{
    self->field_0x1c = 0;
}
