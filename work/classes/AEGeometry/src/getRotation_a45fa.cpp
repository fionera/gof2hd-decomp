#include "class.h"

// AEGeometry::getRotation()  ->  returns Vector(this+0x30..0x38) by value (sret in r0, this in r1)
struct AEGeometry { AbyssEngine::AEMath::Vector getRotation(); };
AbyssEngine::AEMath::Vector AEGeometry::getRotation()
{
    AbyssEngine::AEMath::Vector v;
    v.x = this->f_30;
    v.y = this->f_34;
    v.z = this->f_38;
    return v;
}
