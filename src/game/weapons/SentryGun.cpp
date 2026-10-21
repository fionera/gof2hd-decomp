#include "game/weapons/SentryGun.h"
#include "game/weapons/Gun.h"
#include "game/world/Level.h"
#include "game/ship/KIPlayer.h"
#include "game/ship/Player.h"

SentryGun::SentryGun(Gun *gun, int mesh, int unused, int p4, Level *level)
    : ObjectGun(unused, gun, mesh, 0, level) {
    (void) p4;
    this->cooldown = gun->itemIndex * 3 - 0x279;
}

SentryGun::~SentryGun() {
}

void SentryGun::update(int dt) {
    this->gun->update(dt);
    if (this->gun->hitSmall == 0)
        return;
    this->gun->hitSmall = 0;

    for (int i = this->cooldown; i < this->cooldown + 3; i++) {
        KIPlayer *obj = (*static_cast<Level *>(this->level)->field_b0)[i];
        if (!obj->isDying() &&
            (static_cast<Player *>(obj->player)->isActive() == 0 ||
             static_cast<Player *>(obj->player)->isDead())) {
            static_cast<Level *>(this->level)->field_6c += 1;
            obj->revive();
            obj->setPosition(this->gun->positions.data()[this->gun->currentProjectile]);
            obj->setActive(true);
            return;
        }
    }
}

void SentryGun::render() {
}
