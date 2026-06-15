#include "gof2/game/weapons/SentryGun.h"
#include "gof2/game/weapons/Gun.h"
#include "gof2/game/world/Level.h"
#include "gof2/game/ship/KIPlayer.h"
#include "gof2/game/ship/Player.h"

// SentryGun::SentryGun(Gun*, int, int, int, Level*).
// Runs the ObjectGun base constructor (with reordered arguments) and then seeds the
// spawn index from the gun's item index. The SentryGun vtable is installed by the
// compiler-generated base-init sequence.
SentryGun::SentryGun(Gun* gun, int mesh, int unused, int p4, Level* level)
    : ObjectGun(unused, gun, mesh, 0, level)
{
    (void)p4;
    this->cooldown = gun->itemIndex * 3 - 0x279;
}

// SentryGun::update(int) — virtual override of ObjectGun::update.
// Advances the owned gun, and on a fire pulse re-activates an idle junk object from
// the level's sentry pool, positions it at the current muzzle slot, and spawns it.
void SentryGun::update(int dt)
{
    this->gun->update(dt);

    Gun* gun = this->gun;
    if (gun->hitSmall == 0)
        return;
    gun->hitSmall = 0;

    Level* level = static_cast<Level*>(this->level);
    Array<KIPlayer*>& pool = *level->field_b0;

    int base = this->cooldown;
    for (int i = base; i < base + 3; i++) {
        KIPlayer* obj = pool[i];
        Player* owner = static_cast<Player*>(obj->player);
        if (obj->isDying() == false &&
            (owner->isActive() == 0 || owner->isDead())) {
            level->field_6c += 1;

            void** vtable = *reinterpret_cast<void***>(obj);
            (reinterpret_cast<void (*)(KIPlayer*)>(vtable[0x18 / 4]))(obj);

            Gun* g = this->gun;
            int spawnPos = (int)(intptr_t)(g->positions + g->fireIndex * 12);
            (reinterpret_cast<void (*)(KIPlayer*, int)>(vtable[0x44 / 4]))(obj, spawnPos);
            return;
        }
        base = this->cooldown;
    }
}
