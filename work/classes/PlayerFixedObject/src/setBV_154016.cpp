#include "class.h"

extern "C" void PlayerFixedObject_setBV_arr(PlayerFixedObject *self, Array<BoundingVolume *> *bv)
{
    F<Array<BoundingVolume *> *>(self, 0x128) = bv;
}
