#include "class.h"

// SentryGun::SentryGun(Gun*, int, int, int, Level*) — constructor.
// SentryGun is a polymorphic subclass of ObjectGun: the base initializer runs the
// ObjectGun constructor (with reordered args), clang installs the SentryGun vtable
// (vtable + 8) automatically, then the body initializes the cooldown field at +0xb0.

struct ObjectGun {
    ObjectGun(int owner, Gun *gun, int mesh, unsigned int flags, Level *level);
    virtual ~ObjectGun();
    virtual void update(int);
    virtual void render();
};

struct SentryGun : ObjectGun {
    SentryGun(Gun *gun, int p2, int p3, int p4, Level *level);
    virtual void update(int);
};

SentryGun::SentryGun(Gun *gun, int p2, int p3, int p4, Level *level)
    : ObjectGun(p3, gun, p2, 0, level)
{
    this->f_b0 = gun->f_58 * 3 - 0x279;
}
