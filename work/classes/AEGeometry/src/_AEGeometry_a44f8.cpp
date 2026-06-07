#include "class.h"

// AEGeometry::~AEGeometry() -> delete[] five LOD arrays in order 0x54,0x5c,0x58,0x60,0x64
struct AEGeometry {
    ~AEGeometry();
};
AEGeometry::~AEGeometry()
{
    if (pp(this, 0x54)) operator delete[](pp(this, 0x54));
    pp(this, 0x54) = 0;
    if (pp(this, 0x5c)) operator delete[](pp(this, 0x5c));
    pp(this, 0x5c) = 0;
    if (pp(this, 0x58)) operator delete[](pp(this, 0x58));
    pp(this, 0x58) = 0;
    if (pp(this, 0x60)) operator delete[](pp(this, 0x60));
    pp(this, 0x60) = 0;
    if (pp(this, 0x64)) operator delete[](pp(this, 0x64));
    pp(this, 0x64) = 0;
}
