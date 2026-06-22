#ifndef GOF2_MINEGUN_H
#define GOF2_MINEGUN_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/weapons/ObjectGun.h"

class PlayerEgo;
class Explosion;
class AEGeometry;

class MineGun : public ObjectGun {
public:
    PlayerEgo *player; // owning player (rumble target)
    Array<Explosion *> *explosions; // per-mine explosion pool
    uint8_t *readyFlags; // per-mine "ready to detonate" flags
    AEGeometry *geometry; // mine muzzle geometry
    int field_0xc0; // +0xc0 (zero-initialized, unused)
    int field_0xc4; // +0xc4 (zero-initialized, unused)
    int field_0xc8; // +0xc8 (zero-initialized, unused)
    int rumbleTimer; // +0xcc rumble decay timer
    float rumblePercentage; // +0xd0 current rumble intensity

    MineGun(Gun *gun, int mesh, int param, int unused, Level *level);

    ~MineGun();

    // AbstractGun declares the isRocketGun/isBombGun/isMineGun/isAutoTurret predicate
    // quartet as virtuals (default 0); MineGun is the one that answers isMineGun.
    int isMineGun() override; // overrides AbstractGun::isMineGun (answers yes)
    void render();

    void setPlayer(PlayerEgo *player);

    void update(int delta);
};
#endif
