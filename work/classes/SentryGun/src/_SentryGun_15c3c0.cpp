#include "class.h"

// SentryGun::~SentryGun() — deleting destructor. SentryGun adds no members needing
// cleanup, so the body is just the base ObjectGun destructor followed by a tail-call
// (operator delete). Symbol demangles to contain "~SentryGun".

extern "C" SentryGun *ObjectGun_dtor(SentryGun *self);   // ObjectGun::~ObjectGun(ObjectGun*)
extern "C" void SentryGun_operator_delete(void *p);      // tail thunk

extern "C" void _ZN9SentryGunD0Ev(SentryGun *self)
{
    return SentryGun_operator_delete(ObjectGun_dtor(self));
}
