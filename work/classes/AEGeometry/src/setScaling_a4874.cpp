#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::setScaling(Vector const&) -> tail-call setScaling(v.x, v.y, v.z)
struct AEGeometry {
    void setScaling(float, float, float);
    void setScaling(const Vector &v);
};
void AEGeometry::setScaling(const Vector &v)
{
    return setScaling(v.x, v.y, v.z);
}
