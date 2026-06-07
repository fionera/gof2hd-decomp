#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::setRotation(Vector const&) -> tail-call setRotation(v.x, v.y, v.z)
struct AEGeometry {
    void setRotation(float, float, float);
    void setRotation(const Vector &v);
};
void AEGeometry::setRotation(const Vector &v)
{
    return setRotation(v.x, v.y, v.z);
}
