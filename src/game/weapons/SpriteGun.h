#ifndef GOF2_SPRITEGUN_H
#define GOF2_SPRITEGUN_H
#include <cstdint>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Gun.h"
#include "AbstractGun.h"

class Player;

class Gun;

// Original _ZTV: SpriteGun : AbstractGun (12 slots); overrides setEnemies/setEnemy/update/render, leaves
// translate/replaceGun PURE (__cxa_pure_virtual at slots 6/7), inherits is*. Was standalone (6 slots) in ours.
class SpriteGun : public AbstractGun {
public:
    int32_t field_0x4;
    Gun *gun;

    SpriteGun(Gun *g, int kind);

    ~SpriteGun() override;

    void setEnemies(Array<Player *> *enemies) override;

    void setEnemy(Player *enemy) override;

    void update(int elapsed) override;

    void render() override;
};
#endif
