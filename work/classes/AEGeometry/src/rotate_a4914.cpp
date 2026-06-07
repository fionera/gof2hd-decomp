#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::rotate(Vector const&) -> tail-call rotate(v.x, v.y, v.z)
struct AEGeometry {
    void rotate(float, float, float);
    void rotate(const Vector &v);
};
void AEGeometry::rotate(const Vector &v)
{
    return rotate(v.x, v.y, v.z);
}
