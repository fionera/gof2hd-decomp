#include "gof2/game/weapons/SpriteGun.h"
#include "gof2/game/weapons/Gun.h"

extern "C" void SpriteGun_Gun_update(void *base);
extern "C" void SpriteGun_Gun_setEnemies(void *base);
extern "C" void SpriteGun_Gun_setEnemy(void *base);

// SpriteGun::update(int) — load this->field_8, tail-call Gun::update on the base.

void SpriteGun::update() {
    SpriteGun *this_ = this;
    return SpriteGun_Gun_update(this_->gun);
}

// SpriteGun::setEnemies(Array<Player*>*) — load arg->field_8, tail-call the
// underlying Gun::setEnemies on the embedded base object.

void SpriteGun::setEnemies() {
    SpriteGun *this_ = this;
    return SpriteGun_Gun_setEnemies(this_->gun);
}

// SpriteGun::SpriteGun(Gun*, int) — store (g_SpriteGun + 8) at +0 and 0 at +4.
// g_SpriteGun is a value-typed global loaded PC-relative (hidden visibility) with a
// single dereference; the two word stores fuse into a single strd.
__attribute__((visibility("hidden"))) extern "C" char *const g_SpriteGun;

SpriteGun::SpriteGun(Gun *param_1, int param_2) {
    SpriteGun *this_ = this;
    this_->field_0x0 = g_SpriteGun + 8;
    this_->field_0x4 = 0;
}

// SpriteGun::setEnemy(Player*) — load arg->field_8, tail-call Gun::setEnemy on base.

void SpriteGun::setEnemy() {
    SpriteGun *this_ = this;
    return SpriteGun_Gun_setEnemy(this_->gun);
}
