#include "class.h"

// Returns a Vector by value (sret r0, this r1). Picks the active bounding-volume array,
// indexes it by the stored collision index (this+0x16c) and forwards the chosen BV.
// NEAR: target shares one index+call across both branches (explicit join) and keeps the
// first bv in a callee reg; clang lays the branches out separately here.
struct V3 { float x, y, z; };
extern "C" V3 BV_getProjectionVector(void *bv);

extern "C" V3 PlayerFixedObject_getProjectionVector(PlayerFixedObject *self)
{
    void *bv = self->f_12c;
    if (bv != 0 && self->f_88 == 4) {
        int idx = self->f_16c;
        void **data = *(void ***)((char *)bv + 0x4);
        return BV_getProjectionVector(data[idx]);
    }
    void *bv2 = self->f_128;
    if (bv2 != 0) {
        int idx = self->f_16c;
        void **data = *(void ***)((char *)bv2 + 0x4);
        return BV_getProjectionVector(data[idx]);
    }
    V3 z = {0.0f, 0.0f, 0.0f};
    return z;
}
