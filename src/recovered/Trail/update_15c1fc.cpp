#include "class.h"

using AbyssEngine::AEMath::Vector;

void Trail::update(const Vector &a, const Vector &b)
{
    return update(a.x, a.y, a.z, b.x, b.y, b.z);
}
