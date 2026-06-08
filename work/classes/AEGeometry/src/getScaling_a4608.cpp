#include "class.h"

// AEGeometry::getScaling()  ->  Vector(this+0x3c..0x44) by value
struct AEGeometry { AbyssEngine::AEMath::Vector getScaling(); };
AbyssEngine::AEMath::Vector AEGeometry::getScaling()
{
    AbyssEngine::AEMath::Vector v;
    v.x = f32(this, 0x3c);
    v.y = f32(this, 0x40);
    v.z = f32(this, 0x44);
    return v;
}
