#ifndef GOF2_BOMBGUN_H
#define GOF2_BOMBGUN_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/render/AEGeometry.h"
#include "game/ship/PlayerEgo.h"

#include "game/weapons/RocketGun.h"

#include "engine/math/Vector.h"

class AEGeometry;
class Explosion;
class Gun;
class Level;
class PlayerEgo;

// Steerable/plain bomb. Reuses ObjectGun::secondaryTransform (steerable bomb mesh), rollAngle (rocket
// banking) and useEgoOrientation (player-steered flag) from the base.
class BombGun : public RocketGun {
public:
    AEGeometry *trailGeometry;      // 0xe8 camera target for the steered bomb
    PlayerEgo *player;              // 0xec
    Explosion *explosion;           // 0xf0
    uint32_t geometryTransformId;   // 0xf4
    Vector detonationPosition;      // 0xf8
    uint8_t detonationPending;      // 0x104
    int rumbleTimer;                // 0x108
    float rumbleStrength;           // 0x10c
    Vector cameraOffset;            // 0x110
    Vector cameraTargetOffset;      // 0x11c
    int bombType;                   // 0x128

    BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh, Level *level);

    ~BombGun();

    int isBombGun() override;

    void setPlayer(PlayerEgo *player);

    void render();

    void update(int elapsed);
};
#endif
