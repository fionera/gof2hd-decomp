#ifndef GOF2_MINEGUN_H
#define GOF2_MINEGUN_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "game/ship/PlayerEgo.h"

#include "game/weapons/ObjectGun.h"

class AEGeometry;
class Explosion;
class Gun;
class Level;
class PlayerEgo;

class MineGun : public ObjectGun {
public:
    PlayerEgo *player;
    Array<Explosion *> *explosions;
    uint8_t *readyFlags;
    AEGeometry *geometry;
    int field_0xc0;
    int field_0xc4;
    int field_0xc8;
    int rumbleTimer;
    float rumblePercentage;

    MineGun(Gun *gun, int mesh, int param, int unused, Level *level);

    ~MineGun();

    int isMineGun() override { return 1; }

    void render();

    void setPlayer(PlayerEgo *player);

    void update(int delta);

private:
    // Position of mine i.
    const Vector &minePosition(uint32_t i) {
        return this->gun->hitPositions.data()[i];
    }
};
#endif
