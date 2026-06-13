#ifndef GOF2_BOMBGUN_H
#define GOF2_BOMBGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct BombGun;
struct RocketGun;
struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct LevelScript;
struct Explosion;
struct AEGeometry;
struct PaintCanvas;
struct Transform;
struct TargetFollowCamera;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

// BombGun derives from RocketGun (see RocketGun.h). The fields below 0xe8 live in the
// shared RocketGun/Gun base region; only the ones BombGun itself touches are named here
// (gun/transformId/meshTransformId/bankingAngle/playerControlled/meshId). RocketGun.h names
// the same offsets independently (gun@0x8, transformId@0x10) — kept consistent, not edited.
class BombGun {
public:
    Gun* gun;                           // +0x8   owning Gun (RocketGun base region)
    uint32_t transformId;               // +0x10  root rocket transform handle (base region)
    uint32_t meshTransformId;           // +0x14  simple-mesh transform handle (base region)
    float bankingAngle;                 // +0x20  rocket-cam banking angle (base region)
    uint8_t playerControlled;           // +0x24  player-steered bomb flag (base region)
    uint16_t meshId;                    // +0x28  projectile mesh id (base region)
    AEGeometry* trailGeometry;          // +0xe8  trail / follow-cam geometry
    PlayerEgo* player;                  // +0xec  owning player (set via setPlayer)
    Explosion* explosion;               // +0xf0  detonation effect
    uint32_t geometryTransformId;       // +0xf4  attached-geometry transform (0xffffffff = none)
    Vector detonationPosition;          // +0xf8  world position where the bomb detonates
    uint8_t detonationPending;          // +0x104 set until the bomb has detonated
    int rumbleTimer;                    // +0x108 detonation rumble decay timer (capped 2000)
    float rumbleStrength;               // +0x10c base rumble strength from blast distance
    Vector cameraOffset;                // +0x110 rocket-cam camera offset (0,450,-1400)
    Vector cameraTargetOffset;          // +0x11c rocket-cam target offset (0,0,1700)
    int bombType;                       // +0x128 bomb variant (6=EMP, 0x2a=nuke, ...)

    BombGun(Gun *gun, uint32_t meshId, int rocketArg, int bombType, bool simpleMesh, Level *level);
    void setPlayer(PlayerEgo *player);
    void render();
    void update(int elapsed);
};
#endif
