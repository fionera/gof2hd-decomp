#include "class.h"

// outerCollide(Vector) - Vector passed by value (r1,r2,r3). Pure tail-forward through
// vtable slot 0x3c: ldr r12,[r0]; ldr r12,[r12,#0x3c]; bx r12.
typedef void (*CollideFn)(PlayerFixedObject *, Vector);

extern "C" void PlayerFixedObject_outerCollide_vec(PlayerFixedObject *self, Vector v)
{
    CollideFn fn = *(CollideFn *)(*(char **)self + 0x3c);
    return fn(self, v);
}
