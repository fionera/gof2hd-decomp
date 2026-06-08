#include "class.h"

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

struct ObjectGun {
    ObjectGun(int owner, Gun *gun, int mesh, unsigned int flags, Level *level);
    virtual ~ObjectGun();
    virtual void update(int);
    virtual void render();
};

struct SentryGun : ObjectGun {
    virtual void update(int dt);
};

void SentryGun::update(int dt)
{
    Gun *gun = this->f_8;
    Gun_update(gun, dt);

    gun = this->f_8;
    if (gun->f_4d == 0)
        return;
    gun->f_4d = 0;

    int base = this->f_b0;
    for (int i = base; i < base + 3; i++) {
        PoolObject *obj = F<PoolObject **>(F<void *>(this->f_c, 0xb0), 0x04)[i];
        if (KIPlayer_isDying((KIPlayer *)obj) == 0 &&
            (Player_isActive(obj->owner) == 0 || Player_isDead(obj->owner) != 0)) {
            F<int>(this->f_c, 0x6c) += 1;
            ((void (*)(PoolObject *))obj->vtable[0x18 / 4])(obj);
            Gun *g = this->f_8;
            ((void (*)(PoolObject *, int))obj->vtable[0x44 / 4])(
                obj, g->f_c + g->f_a0 * 12);
            return SentryGun_fire_tail(obj, 1);
        }
        base = this->f_b0;
    }
}
