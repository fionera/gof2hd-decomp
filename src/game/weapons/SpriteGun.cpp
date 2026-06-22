#include "game/weapons/SpriteGun.h"
#include "game/weapons/Gun.h"


extern void (*SpriteGun_enemiesHandler_slot)(void *);


extern void (*SpriteGun_enemyHandler_slot)(void *);


extern void (*SpriteGun_updateHandler_slot)(void *);

SpriteGun::SpriteGun(Gun * /*gun*/, int /*kind*/) {
    this->field_0x4 = 0;
}

SpriteGun::~SpriteGun() {
}

void SpriteGun::setEnemies(Array<Player *> *enemies) {
    SpriteGun_enemiesHandler_slot(enemies->data());
}

void SpriteGun::setEnemy(Player *enemy) {
    SpriteGun_enemyHandler_slot(reinterpret_cast<void **>(enemy)[2]);
}

void SpriteGun::update(int elapsed) {
    SpriteGun_updateHandler_slot(reinterpret_cast<void **>(elapsed)[2]);
}

void SpriteGun::render() {
}
