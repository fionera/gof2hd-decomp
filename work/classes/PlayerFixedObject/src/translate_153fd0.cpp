#include "class.h"

typedef void (*SetPosFn)(PlayerFixedObject *, float, float, float);

// Reads ship pos (signed ints at +0x178/0x17c/0x180 -> float), adds the delta Vector,
// then forwards to vtable slot 0x48.
extern "C" void PlayerFixedObject_translate(PlayerFixedObject *self, const Vector &d)
{
    float x = (float)F<int32_t>(self, 0x178);
    float y = (float)F<int32_t>(self, 0x17c);
    float z = (float)F<int32_t>(self, 0x180);
    SetPosFn fn = *(SetPosFn *)(*(char **)self + 0x48);
    return fn(self, d.x + x, d.y + y, d.z + z);
}
