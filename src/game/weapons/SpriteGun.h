#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include <cstdint>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Gun.h"

class Player;


class Gun;


class SpriteGun {
public:
    int32_t field_0x4;

    SpriteGun(Gun *gun, int kind);

    virtual ~SpriteGun();

    virtual void setEnemies(Array<Player *> *enemies);

    virtual void setEnemy(Player *enemy);

    virtual void update(int elapsed);

    virtual void render();
};
#endif
