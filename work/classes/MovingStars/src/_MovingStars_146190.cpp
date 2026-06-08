#include "class.h"

// MovingStars::~MovingStars() -> delete[] four arrays in order 0x0,0x4,0xc,0x10; returns this.
struct MovingStars {
    ~MovingStars();
};
MovingStars::~MovingStars()
{
    if (pp(this, 0x0)) operator delete[](pp(this, 0x0));
    pp(this, 0x0) = 0;
    if (pp(this, 0x4)) operator delete[](pp(this, 0x4));
    pp(this, 0x4) = 0;
    if (pp(this, 0xc)) operator delete[](pp(this, 0xc));
    pp(this, 0xc) = 0;
    if (pp(this, 0x10)) operator delete[](pp(this, 0x10));
    pp(this, 0x10) = 0;
}
