#include "class.h"

// getCollisionNormal(Vector&): zero the three components of the passed vector.
void BoundingVolume::getCollisionNormal(const Vector &out)
{
    Vector *v = (Vector *)&out;
    *(uint32_t *)((char *)v + 0) = 0;
    *(uint32_t *)((char *)v + 4) = 0;
    *(uint32_t *)((char *)v + 8) = 0;
}
