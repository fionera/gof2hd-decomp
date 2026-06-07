#include "class.h"

// PlayerFighter complete-object destructor (D1). Returns this. Sets the vtable, destroys the
// owned members, then tail-calls the base (Fighter/Player) destructor.
extern "C" char PlayerFighter_vtable;   // vtable symbol base
extern "C" void *Route_dtor(void *p);
extern "C" void ArrayReleaseClasses_BV(void *arr);
extern "C" void *ArrayBV_dtor(void *p);
extern "C" void *Trail_dtor(void *p);
extern "C" void *Explosion_dtor(void *p);
extern "C" void *EaseInOutMatrix_dtor(void *p);
extern "C" void operator_delete(void *p);
extern "C" void *PlayerFighter_base_dtor(PlayerFighter *self);

extern "C" void *_ZN13PlayerFighterD1Ev(PlayerFighter *self)
{
    *(void **)self = &PlayerFighter_vtable + 8;

    void *r = F<void *>(self, 0x144);
    if (r != 0) operator_delete(Route_dtor(r));
    F<void *>(self, 0x144) = 0;

    void *bv = F<void *>(self, 0x150);
    if (bv != 0) {
        ArrayReleaseClasses_BV(bv);
        void *bv2 = F<void *>(self, 0x150);
        if (bv2 != 0) operator_delete(ArrayBV_dtor(bv2));
    }
    F<void *>(self, 0x150) = 0;

    void *t = F<void *>(self, 0x154);
    if (t != 0) operator_delete(Trail_dtor(t));
    F<void *>(self, 0x154) = 0;

    void *e = F<void *>(self, 0x124);
    if (e != 0) operator_delete(Explosion_dtor(e));
    F<void *>(self, 0x124) = 0;

    void *m = F<void *>(self, 0x2b8);
    if (m != 0) operator_delete(EaseInOutMatrix_dtor(m));
    F<void *>(self, 0x2b8) = 0;

    return PlayerFighter_base_dtor(self);
}
