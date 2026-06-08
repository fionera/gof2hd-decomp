#include "class.h"

Vector BoundingAAB::getCollisionNormal(const Vector &)
{
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}
