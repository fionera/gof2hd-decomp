#include "class.h"

extern "C" void *operator_new(uint32_t);
extern "C" void Array_BV_ctor(void *arr);
extern "C" void BoundingVolume_setArr(BoundingVolume *bv, void *arr);

// Allocate a 12-byte Array<BoundingVolume*>, default-construct, store at +0x128,
// then forward (param_1, arr) to the bounding-volume registration thunk.
// NEAR: clang assigns arr->r5, this->r6 (we get them swapped); allocator tie-break, not source-driven.
extern "C" void PlayerFixedObject_setBV(PlayerFixedObject *self, BoundingVolume *bv)
{
    void *arr = operator_new(0xc);
    Array_BV_ctor(arr);
    F<void *>(self, 0x128) = arr;
    return BoundingVolume_setArr(bv, arr);
}
