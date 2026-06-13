#include "gof2/game/weapons/SentryGun.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ship/Player.h"
#include "gof2/game/weapons/Gun.h"

// SentryGun::~SentryGun() — deleting destructor. SentryGun adds no members needing
// cleanup, so the body is just the base ObjectGun destructor followed by a tail-call
// (operator delete). Symbol demangles to contain "~SentryGun".

extern "C" SentryGun *ObjectGun_dtor(SentryGun *self);   // ObjectGun::~ObjectGun(ObjectGun*)

void _ZN9SentryGunD0Ev(SentryGun *self)
{
    return ::operator delete(ObjectGun_dtor(self));
}

// SentryGun::SentryGun(Gun*, int, int, int, Level*) — constructor.
// SentryGun is a polymorphic subclass of ObjectGun: the base initializer runs the
// ObjectGun constructor (with reordered args), clang installs the SentryGun vtable
// (vtable + 8) automatically, then the body initializes the cooldown field at +0xb0.

extern "C" void ObjectGun_ctor(SentryGun *self, int owner, Gun *gun, int mesh,
                               unsigned int flags, Level *level);

SentryGun * SentryGun::ctor(Gun *gun, int p2, int p3, int p4, Level *level) {
    SentryGun *self = this;
    ObjectGun_ctor(self, p3, gun, p2, 0, level);
    self->cooldown = gun->itemIndex * 3 - 0x279;
    return self;
}

// SentryGun::update(int) — virtual override of ObjectGun::update.
// Advances the owned Gun, and on a fire-pulse spawns from a pool of objects.

struct KIPlayer;
struct Player;

// Gun::update(int)
// KIPlayer::isDying()
// Player::isActive()
// Player::isDead()
// tail thunk

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

// ---- SentryGun::fire_tail @ b.w 0x1abe08 (dynamic import veneer) ----------
// Final hand-off of the freshly spawned pool object to the load-time-resolved
// activation import. No static body exists for the import (pure PLT veneer),
// so this forwards the (object, flag) tail-call exactly as the disassembly does.
// 0x1abe08 is PlayerJunk::reset; the flag is consumed there (hardcoded show==1).
class PlayerJunk;
extern void _ZN10PlayerJunk5resetEv(PlayerJunk *self);
void SentryGun::fire_tail(void *obj, int flag) {
    (void)flag;
    _ZN10PlayerJunk5resetEv((PlayerJunk *)obj);
}

void SentryGun::update(int dt) {
    SentryGun *self = this;
    Gun *gun = self->gun;
    ((Gun *)(gun))->update(dt);

    gun = self->gun;
    if (gun->field_0x4d == 0)
        return;
    gun->field_0x4d = 0;

    SentryLevel *level = (SentryLevel *)self->level;

    int base = self->cooldown;
    for (int i = base; i < base + 3; i++) {
        PoolObject *obj = level->pool[i];
        if (((KIPlayer *)((KIPlayer *)obj))->isDying() == 0 &&
            (((Player *)(obj->owner))->isActive() == 0 || ((Player *)(obj->owner))->isDead() != 0)) {
            level->spawnCount += 1;
            ((void (*)(PoolObject *))obj->vtable[0x18 / 4])(obj);
            Gun *g = self->gun;
            ((void (*)(PoolObject *, int))obj->vtable[0x44 / 4])(
                obj, (int)(intptr_t)((char *)g->positions + g->field_0xa0 * 12));
            return SentryGun::fire_tail(obj, 1);
        }
        base = self->cooldown;
    }
}
