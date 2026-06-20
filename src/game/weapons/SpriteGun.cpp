#include "game/weapons/SpriteGun.h"
#include "game/weapons/Gun.h"

// setEnemies/setEnemy/update forward the relevant pointer through the inherited
// gun-hierarchy handlers the dynamic linker resolves into engine relocation
// slots; they have no static body in this image, so they stay as resolved-slot
// externs.
__attribute__((visibility("hidden"))) extern void (*SpriteGun_enemiesHandler_slot)(void*);
__attribute__((visibility("hidden"))) extern void (*SpriteGun_enemyHandler_slot)(void*);
__attribute__((visibility("hidden"))) extern void (*SpriteGun_updateHandler_slot)(void*);

SpriteGun::SpriteGun(Gun* /*gun*/, int /*kind*/)
{
    this->field_0x4 = 0;
}

SpriteGun::~SpriteGun()
{
}

void SpriteGun::setEnemies(Array<Player*>* enemies)
{
    SpriteGun_enemiesHandler_slot(enemies->data());
}

void SpriteGun::setEnemy(Player* enemy)
{
    SpriteGun_enemyHandler_slot(reinterpret_cast<void**>(enemy)[2]);
}

void SpriteGun::update(int elapsed)
{
    SpriteGun_updateHandler_slot(reinterpret_cast<void**>(elapsed)[2]);
}

void SpriteGun::render()
{
}
