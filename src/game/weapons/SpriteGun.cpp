#include "gof2/game/weapons/SpriteGun.h"
#include "gof2/game/weapons/Gun.h"

// Per-instance vtable holder resolved by the engine at load time.
__attribute__((visibility("hidden"))) extern void* SpriteGun_vtable;

// update/setEnemies/setEnemy forward through the inherited gun-hierarchy handlers
// the dynamic linker resolves into engine relocation slots; they have no static
// body in this image, so they stay as resolved-slot externs.
__attribute__((visibility("hidden"))) extern void (*SpriteGun_updateHandler_slot)(void*);
__attribute__((visibility("hidden"))) extern void (*SpriteGun_enemiesHandler_slot)(void*);
__attribute__((visibility("hidden"))) extern void (*SpriteGun_enemyHandler_slot)(void*);

SpriteGun::SpriteGun(Gun* gun, int kind)
{
    (void)kind;
    this->vtable = (char*)SpriteGun_vtable + 8;
    this->field_0x4 = 0;
    this->gun = gun;
}

void SpriteGun::update(int elapsed)
{
    (void)elapsed;
    SpriteGun_updateHandler_slot(this->gun);
}

void SpriteGun::setEnemies(Array<Player*>* enemies)
{
    (void)enemies;
    SpriteGun_enemiesHandler_slot(this->gun);
}

void SpriteGun::setEnemy(Player* enemy)
{
    (void)enemy;
    SpriteGun_enemyHandler_slot(this->gun);
}
