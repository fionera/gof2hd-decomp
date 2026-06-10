#include "SpriteGun.h"


extern "C" void SpriteGun_Gun_update(void *base);
extern "C" void SpriteGun_Gun_setEnemies(void *base);
extern "C" void SpriteGun_Gun_setEnemy(void *base);

// ---- update_169abe.cpp ----
// SpriteGun::update(int) — load this->field_8, tail-call Gun::update on the base.

extern "C" void SpriteGun_update(SpriteGun *this_)
{
    return SpriteGun_Gun_update(F<void *>(this_, 0x8));
}

// ---- setEnemies_169ac4.cpp ----
// SpriteGun::setEnemies(Array<Player*>*) — load arg->field_8, tail-call the
// underlying Gun::setEnemies on the embedded base object.

extern "C" void SpriteGun_setEnemies(Array *param_1)
{
    return SpriteGun_Gun_setEnemies(F<void *>(param_1, 0x8));
}

// ---- SpriteGun_169aa4.cpp ----
// SpriteGun::SpriteGun(Gun*, int) — store (g_SpriteGun + 8) at +0 and 0 at +4.
// g_SpriteGun is a value-typed global loaded PC-relative (hidden visibility) with a
// single dereference; the two word stores fuse into a single strd.
__attribute__((visibility("hidden"))) extern "C" char *const g_SpriteGun;

extern "C" void SpriteGun_SpriteGun(SpriteGun *this_, Gun *param_1, int param_2)
{
    F<char *>(this_, 0x0) = g_SpriteGun + 8;
    F<int>(this_, 0x4) = 0;
}

// ---- setEnemy_169aca.cpp ----
// SpriteGun::setEnemy(Player*) — load arg->field_8, tail-call Gun::setEnemy on base.

extern "C" void SpriteGun_setEnemy(Player *param_1)
{
    return SpriteGun_Gun_setEnemy(F<void *>(param_1, 0x8));
}
