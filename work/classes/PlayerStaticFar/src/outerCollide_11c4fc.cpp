#include "class.h"

// outerCollide(float,float,float): tail-call the virtual slot at vtable+0x38,
// forwarding the same (this,x,y,z) arguments unchanged.
typedef bool (*OuterCollideFn)(PlayerStaticFar *self, float x, float y, float z);

bool PlayerStaticFar::outerCollide(float x, float y, float z)
{
    OuterCollideFn fn = *(OuterCollideFn *)(*(char **)this + 0x38);
    return fn(this, x, y, z);
}
