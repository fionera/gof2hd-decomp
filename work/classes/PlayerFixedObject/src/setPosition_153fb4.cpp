#include "class.h"

typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

extern "C" void PlayerFixedObject_setPosition_vec(PlayerFixedObject *self, const Vector &v)
{
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, v.x, v.y, v.z);
}
