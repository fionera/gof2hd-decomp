#include "class.h"

extern "C" void *operator_new(uint32_t size);                       // 0x6eb24
extern "C" void ArrayBV_ctor(void *a);                             // 0x730c0  Array<BoundingVolume*>::Array()
// Tail-call: copy the source volume's children into the new array.
extern "C" void BoundingVolume_setVolume_tail(BoundingVolume *src, void *arr);  // 0x1ac218

void BoundingVolume::setVolume(BoundingVolume *src)
{
    void *arr = operator_new(0xc);
    ArrayBV_ctor(arr);
    this->f_4 = arr;
    return BoundingVolume_setVolume_tail(src, arr);
}
