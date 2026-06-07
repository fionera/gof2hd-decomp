#include "class.h"

// Returns the init position (fields at +0x58,+0x5c,+0x60) by value: sret in r0,
// this in r1. ldrd/strd pair the first two floats; the third is a single str.
Vector PlayerStaticFar::getInitPosition(Vector)
{
    Vector r;
    r.x = f32(this, 0x58);
    r.y = f32(this, 0x5c);
    r.z = f32(this, 0x60);
    return r;
}
