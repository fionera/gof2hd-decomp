#include "class.h"

// KIPlayer::setPosition(float, float, float)
//   Packs the three floats into a Vector and dispatches through vtable slot +0x44
//   (the virtual setPosition(Vector const&)).
extern "C" void KIPlayer_setPosition3(KIPlayer *self, float x, float y, float z)
{
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    void **vtbl = *(void ***)self;
    typedef void (*fn_t)(KIPlayer *, Vector *);
    ((fn_t)vtbl[0x44 / 4])(self, &v);
}
