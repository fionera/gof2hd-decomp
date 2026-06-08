#include "class.h"

// AEGeometry::getRotation()  ->  returns Vector(this+0x30..0x38) by value (sret in r0, this in r1)
struct AEGeometry { AbyssEngine::AEMath::Vector getRotation(); };
AbyssEngine::AEMath::Vector AEGeometry::getRotation()
{
    AbyssEngine::AEMath::Vector v;
    v.x = f32(this, 0x30);
    v.y = f32(this, 0x34);
    v.z = f32(this, 0x38);
    return v;
}
