#ifndef GOF2_BOMBGUN_H
#define GOF2_BOMBGUN_H
#include "gof2/common.h"
#include "gof2/game/weapons/RocketGun.h"

class AEGeometry;
class Explosion;
class PlayerEgo;
class Level;
class Gun;

// A guided "bomb" projectile (nuke / EMP / mine). It extends RocketGun with a
// detonation Explosion effect, a player-steerable rocket camera, and post-blast
// rumble feedback.
class BombGun : public RocketGun {
public:
    // RocketGun/Gun base region fields that BombGun reads but the RocketGun
    // header does not model (gun and transformId are inherited).
    uint32_t meshTransformId;       // simple-mesh transform handle
    float bankingAngle;             // rocket-cam banking angle
    uint8_t playerControlled;       // player-steered bomb flag
    uint16_t meshId;                // projectile mesh id

    AEGeometry* trailGeometry;      // trail / follow-cam geometry
    PlayerEgo* player;              // owning player (set via setPlayer)
    Explosion* explosion;           // detonation effect
    uint32_t geometryTransformId;   // attached-geometry transform (0xffffffff = none)
    Vector detonationPosition;      // world position where the bomb detonates
    uint8_t detonationPending;      // set until the bomb has detonated
    int rumbleTimer;                // detonation rumble decay timer (capped 2000)
    float rumbleStrength;           // base rumble strength from blast distance
    Vector cameraOffset;            // rocket-cam camera offset (0,450,-1400)
    Vector cameraTargetOffset;      // rocket-cam target offset (0,0,1700)
    int bombType;                   // bomb variant (6=EMP, 0x2a=nuke, ...)

    BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh, Level *level);
    ~BombGun();

    void setPlayer(PlayerEgo *player);
    void render();
    void update(int elapsed);
};
#endif
