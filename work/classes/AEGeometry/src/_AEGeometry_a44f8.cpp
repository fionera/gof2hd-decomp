#include "class.h"

// AEGeometry::~AEGeometry() -> delete[] five LOD arrays in order 0x54,0x5c,0x58,0x60,0x64
struct AEGeometry {
    ~AEGeometry();
};
AEGeometry::~AEGeometry()
{
    if (this->f_54) operator delete[](this->f_54);
    this->f_54 = 0;
    if (this->f_5c) operator delete[](this->f_5c);
    this->f_5c = 0;
    if (this->f_58) operator delete[](this->f_58);
    this->f_58 = 0;
    if (this->f_60) operator delete[](this->f_60);
    this->f_60 = 0;
    if (this->f_64) operator delete[](this->f_64);
    this->f_64 = 0;
}
