#include "gof2/SentryGun.h"
#include "gof2/Gun.h"




// ---- _SentryGun_15c3c0.cpp ----
// SentryGun::~SentryGun() — deleting destructor. SentryGun adds no members needing
// cleanup, so the body is just the base ObjectGun destructor followed by a tail-call
// (operator delete). Symbol demangles to contain "~SentryGun".

extern "C" SentryGun *ObjectGun_dtor(SentryGun *self);   // ObjectGun::~ObjectGun(ObjectGun*)
extern "C" void SentryGun_operator_delete(void *p);      // tail thunk

extern "C" void _ZN9SentryGunD0Ev(SentryGun *self)
{
    return SentryGun_operator_delete(ObjectGun_dtor(self));
}

// ---- SentryGun_15c380.cpp ----
// SentryGun::SentryGun(Gun*, int, int, int, Level*) — constructor.
// SentryGun is a polymorphic subclass of ObjectGun: the base initializer runs the
// ObjectGun constructor (with reordered args), clang installs the SentryGun vtable
// (vtable + 8) automatically, then the body initializes the cooldown field at +0xb0.

extern "C" void ObjectGun_ctor(SentryGun *self, int owner, Gun *gun, int mesh,
                               unsigned int flags, Level *level);

extern "C" SentryGun *SentryGun_ctor(SentryGun *self, Gun *gun, int p2, int p3,
                                     int p4, Level *level)
{
    ObjectGun_ctor(self, p3, gun, p2, 0, level);
    self->field_0xb0 = gun->field_0x58 * 3 - 0x279;
    return self;
}

// ---- update_15c3d2.cpp ----
// SentryGun::update(int) — virtual override of ObjectGun::update.
// Advances the owned Gun, and on a fire-pulse spawns from a pool of objects.

struct KIPlayer;
struct Player;

extern "C" void Gun_update(Gun *self, int dt);                       // Gun::update(int)
extern "C" int KIPlayer_isDying(KIPlayer *self);                     // KIPlayer::isDying()
extern "C" unsigned char Player_isActive(Player *self);             // Player::isActive()
extern "C" int Player_isDead(Player *self);                         // Player::isDead()
extern "C" void SentryGun_fire_tail(void *obj, int flag);          // tail thunk

// Object pulled from the pool: polymorphic, called through its vtable.
struct PoolObject {
    void **vtable;
    Player *owner;   // +0x04
};

// The Level held in field_0xc exposes a pool array at +0xb0 and a spawn counter at +0x6c.
struct SentryLevel {
    char pad[0x6c];
    int spawnCount;     // +0x6c
    char pad2[0xb0 - 0x70];
    PoolObject **pool;  // +0xb0
};

extern "C" void SentryGun_update(SentryGun *self, int dt)
{
    Gun *gun = self->field_0x8;
    Gun_update(gun, dt);

    gun = self->field_0x8;
    if (gun->field_0x4d == 0)
        return;
    gun->field_0x4d = 0;

    SentryLevel *level = (SentryLevel *)self->field_0xc;

    int base = self->field_0xb0;
    for (int i = base; i < base + 3; i++) {
        PoolObject *obj = level->pool[i];
        if (KIPlayer_isDying((KIPlayer *)obj) == 0 &&
            (Player_isActive(obj->owner) == 0 || Player_isDead(obj->owner) != 0)) {
            level->spawnCount += 1;
            ((void (*)(PoolObject *))obj->vtable[0x18 / 4])(obj);
            Gun *g = self->field_0x8;
            ((void (*)(PoolObject *, int))obj->vtable[0x44 / 4])(
                obj, (int)(intptr_t)((char *)g->field_0xc + g->field_0xa0 * 12));
            return SentryGun_fire_tail(obj, 1);
        }
        base = self->field_0xb0;
    }
}
