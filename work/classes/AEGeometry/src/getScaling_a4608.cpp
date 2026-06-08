#include "class.h"

// AEGeometry::getScaling()  ->  Vector(this+0x3c..0x44) by value
struct AEGeometry { AbyssEngine::AEMath::Vector getScaling(); };
AbyssEngine::AEMath::Vector AEGeometry::getScaling()
{
    AbyssEngine::AEMath::Vector v;
    v.x = this->f_3c;
    v.y = this->f_40;
    v.z = this->f_44;
    return v;
}
