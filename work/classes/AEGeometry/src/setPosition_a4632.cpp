#include "class.h"

using namespace AbyssEngine::AEMath;

// AEGeometry::setPosition(Vector const&) -> tail-call ext(this, v.x, v.y, v.z)
extern "C" void _ae_setPosition3(void *self, float x, float y, float z);

struct AEGeometry { void setPosition(const Vector &v); };
void AEGeometry::setPosition(const Vector &v)
{
    return _ae_setPosition3(this, v.x, v.y, v.z);
}
