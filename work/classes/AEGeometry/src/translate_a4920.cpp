#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::translate(Vector const&) -> tail-call translate(v.x, v.y, v.z)
struct AEGeometry {
    void translate(float, float, float);
    void translate(const Vector &v);
};
void AEGeometry::translate(const Vector &v)
{
    return translate(v.x, v.y, v.z);
}
