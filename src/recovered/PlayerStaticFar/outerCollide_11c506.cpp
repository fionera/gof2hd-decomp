#include "class.h"

// outerCollide(Vector): the Vector is passed in r0..? In the target this is a
// 3-instruction thunk: load vtable, fetch slot+0x38, tail-call with args
// already in place. Model as a tail-call through the virtual slot forwarding
// the same registers.
typedef void (*OuterCollideVecFn)(PlayerStaticFar *self, float x, float y, float z);

void PlayerStaticFar::outerCollide(Vector value)
{
    OuterCollideVecFn fn = *(OuterCollideVecFn *)(*(char **)this + 0x38);
    return fn(this, value.x, value.y, value.z);
}
