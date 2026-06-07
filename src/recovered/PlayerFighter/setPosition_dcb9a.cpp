#include "class.h"

// setPosition(Vector const&) — virtual dispatch through vtable slot 0x48 with the
// three unpacked vector components.
typedef void (*SetPosFn)(PlayerFighter *, float, float, float);

extern "C" void PlayerFighter_setPosition_ref(PlayerFighter *self, const Vector &v)
{
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, v.x, v.y, v.z);
}
