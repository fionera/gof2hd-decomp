#include "class.h"

struct BoundingVolume;
extern "C" void *operator_new(unsigned int sz);
extern "C" void ArrayBV_ctor(Array<BoundingVolume *> *a);
extern "C" void PlayerFighter_setBV_add(BoundingVolume *bv, Array<BoundingVolume *> *a);

extern "C" void PlayerFighter_setBV_b(PlayerFighter *self, BoundingVolume *bv)
{
    Array<BoundingVolume *> *a = (Array<BoundingVolume *> *)operator_new(0xc);
    ArrayBV_ctor(a);
    F<Array<BoundingVolume *> *>(self, 0x150) = a;
    return PlayerFighter_setBV_add(bv, a);
}
