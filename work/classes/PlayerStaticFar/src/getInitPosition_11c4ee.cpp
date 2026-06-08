#include "class.h"

// Returns the init position (fields at +0x58,+0x5c,+0x60) by value: sret in r0,
// this in r1. ldrd/strd pair the first two floats; the third is a single str.
Vector PlayerStaticFar::getInitPosition(Vector)
{
    Vector r;
    r.x = this->f_58;
    r.y = this->f_5c;
    r.z = this->f_60;
    return r;
}
