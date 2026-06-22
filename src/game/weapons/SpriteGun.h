#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include <cstdint>
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

class Gun;
class Player;

class SpriteGun {
public:
    int32_t field_0x4; // zero-initialised; semantics not recovered

    SpriteGun(Gun *gun, int kind);

    virtual ~SpriteGun();

    // Virtual methods in vtable order: setEnemies, setEnemy, update, render.
    virtual void setEnemies(Array<Player *> *enemies);

    virtual void setEnemy(Player *enemy);

    virtual void update(int elapsed);

    virtual void render();
};
#endif
