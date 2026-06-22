#ifndef GOF2_BOMBGUN_H
#define GOF2_BOMBGUN_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"
#include "engine/render/AEGeometry.h"
#include "game/ship/PlayerEgo.h"

#include "game/weapons/RocketGun.h"

class BombGun : public RocketGun {
public:
    uint32_t meshTransformId;
    float bankingAngle;
    uint8_t playerControlled;
    uint16_t meshId;

    AEGeometry *trailGeometry;
    PlayerEgo *player;
    Explosion *explosion;
    uint32_t geometryTransformId;
    Vector detonationPosition;
    uint8_t detonationPending;
    int rumbleTimer;
    float rumbleStrength;
    Vector cameraOffset;
    Vector cameraTargetOffset;
    int bombType;

    BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh, Level *level);

    ~BombGun();

    int isBombGun() override;

    void setPlayer(PlayerEgo *player);

    void render();

    void update(int elapsed);
};
#endif
