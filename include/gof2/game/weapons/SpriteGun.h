#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- SpriteGun: a lightweight sprite-based weapon wrapper around
// a Gun. It forwards update and enemy-targeting to the owned gun. Top-level class
// (no namespace).

class Gun;
class Player;

class SpriteGun {
public:
    void* vtable;
    int field_0x4;                      // zero-initialised; semantics not recovered
    Gun* gun;                           // owned weapon

    SpriteGun(Gun* gun, int kind);

    void update(int elapsed);
    void setEnemies(Array<Player*>* enemies);
    void setEnemy(Player* enemy);
};
#endif
