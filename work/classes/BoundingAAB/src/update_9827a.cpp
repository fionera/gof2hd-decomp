#include "class.h"

extern "C" void BoundingVolume_update(BoundingAAB *self, float x, float y, float z);

void BoundingAAB::update(float x, float y, float z)
{
    return BoundingVolume_update(this, x, y, z);
}
