#include "class.h"

struct BoundingVolume;

extern "C" void PlayerFighter_setBV_a(PlayerFighter *self, Array<BoundingVolume *> *v)
{
    F<Array<BoundingVolume *> *>(self, 0x150) = v;
}
