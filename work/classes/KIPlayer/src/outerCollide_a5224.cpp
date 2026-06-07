#include "class.h"

typedef void (*CollideFn)(KIPlayer *, float, float, float);

extern "C" void KIPlayer_outerCollide(KIPlayer *self, const Vector &v)
{
    CollideFn fn = *(CollideFn *)(*(char **)self + 0x3c);
    return fn(self, v.x, v.y, v.z);
}
