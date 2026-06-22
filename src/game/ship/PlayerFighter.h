#ifndef GOF2_PLAYERFIGHTER_H
#define GOF2_PLAYERFIGHTER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "game/ship/KIPlayer.h"

struct BoundingVolume;
class AEGeometry;
class Level;
class Route;
class Explosion;
class Trail; // global ::Trail (engine/render/Trail.h)
namespace AbyssEngine {
    class EaseInOutMatrix;
}

class PlayerFighter : public KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;
    using Matrix = AbyssEngine::AEMath::Matrix;

    int32_t player;
    int32_t geometry;
    int32_t subGeometry;
    uint8_t field_0x24;
    signed char field_0x25;
    int32_t wingmanCommand;
    Vector renderPosition; // world position synced from geometry each frame
    int32_t field_0x38;
    signed char field_0x43;
    uint8_t crateCaptured; // +0x4c
    Array<int> *lootList; // loot item-id list (or mission-crate item)
    int32_t level;
    float posX; // cached spawn/target position (decompiled as int32_t holding float bits)
    float posY;
    float posZ;
    uint8_t missionCrateLost;
    uint8_t missionCrateCaptured;
    uint8_t crateLost;
    Route *routeClone;
    uint8_t exhaustHidden; // +0x74  (Ghidra: exhaustVisible flag)
    AEGeometry *wreckGeometry; // +0x78
    int32_t field_0x80;
    int32_t field_0x84;
    int32_t state;
    Vector resetVecA; // zeroed on reset() (transient steering vector)
    uint8_t isMissionCrate;
    int32_t deathTimer;
    signed char field_0xdc;
    int32_t field_0xe4;
    int32_t field_0xf0;
    uint8_t field_0xf5;
    int32_t fov;
    signed char rollActive;
    int32_t pushTimer;
    int32_t pushDuration;
    Vector pushNormal; // unit push direction
    Vector pushImpulse; // scaled push offset
    Explosion *explosion;
    int32_t field_0x128;
    uint8_t field_0x12c;
    short field_0x12d;
    uint8_t field_0x12e;
    signed char field_0x12f;
    int32_t field_0x130;
    int32_t field_0x134;
    int32_t field_0x138;
    signed char field_0x13c;
    uint8_t field_0x13d;
    signed char field_0x13e;
    int32_t field_0x140;
    Route *route;
    int32_t field_0x148;
    Route *commandRoute;
    Array<BoundingVolume *> *boundingVolumes; // active bounding volumes (owned)
    Trail *trail;
    Vector workingPosition; // current logical position (spawn-relative)
    Vector resetVecB; // zeroed on reset() (transient steering vector)
    Vector resetVecC; // zeroed on reset() (transient steering vector)
    int32_t engineTrailSystem; // +0x1a0
    float rotate;
    float shootError;
    float speed;
    int32_t field_0x1b0;
    int32_t boostProb;
    int32_t maneuverTimer; // +0x1b8
    int32_t field_0x1c0;
    int32_t field_0x1c4;
    int32_t field_0x1c8;
    int32_t deltaTime;
    int32_t deltaTimeHi;
    int32_t hitpoints;
    int32_t field_0x1dc;
    signed char field_0x1e0;
    int32_t field_0x1e4;
    float currentSpeed;
    int32_t field_0x1ec;
    float currentRotate;
    short field_0x1f4;
    int32_t field_0x1f8;
    signed char field_0x1fc;
    int32_t field_0x200;
    int32_t field_0x204;
    int32_t field_0x208;
    int32_t targetRoll; // +0x20c
    int32_t smoothRoll; // +0x210
    int32_t field_0x214;
    Matrix easeBaseMatrix; // matrix constructed in ctor (ease/transform base)
    Matrix rollMatrix; // barrel-roll bank matrix (built in roll())
    signed char field_0x254;
    signed char field_0x255;
    int32_t field_0x294;
    int32_t field_0x298;
    int32_t rollSamples; // +0x29c  (ring buffer base)
    int32_t field_0x2a0;
    int32_t field_0x2a4;
    int32_t field_0x2a8;
    int32_t field_0x2ac;
    int32_t rollSampleIndex; // +0x2b0
    uint8_t rollBufferFilled; // +0x2b4
    AbyssEngine::EaseInOutMatrix *easeMatrix;
    int32_t spacePoint; // +0x2c4
    int32_t cloakTimer;
    int32_t field_0x2c9;
    int32_t cloakDuration;
    int32_t field_0x2cd;
    uint8_t cloakActive;
    int32_t cloakCooldown;
    uint8_t cloakingPossible;
    signed char field_0x2d9;
    uint32_t cloakMaterial;
    int32_t cloakSavedMode; // +0x2e0  (saved material mode, restored on uncloak)
    uint8_t aiDisabled;
    int32_t gunSwitchTimer; // +0x2e8

    PlayerFighter(int faction, int wingmanCmd, Player *player, AEGeometry *geom,
                  float x, float y, float z, bool active);

    ~PlayerFighter();

    void awake() override;

    void reset();

    void revive() override;

    void cloak(int dur, bool b);

    int collide(float x, float y, float z) override; // actor vtable slot +0x38
    void handleCloaking();

    uint8_t hasCrateCaptured();

    uint8_t hasCrateLost();

    uint8_t hasMissionCrateCaptured();

    uint8_t hasMissionCrateLost();

    void initPush(const Vector &target, int radius) override; // actor vtable slot +0x2c
    int outerCollide(float x, float y, float z) override; // actor vtable slot +0x3c
    void push(int dt) override;

    void removeTrail();

    void render() override;

    void roll(int angle);

    void setAIDisabled(bool v);

    void setBV(Array<BoundingVolume *> *v);

    void setBV(BoundingVolume *bv);

    void setBoostProb(int v);

    void setCloakingPossible(bool v);

    void setExhaustVisible(bool vis);

    void setLevel(Level *lvl);

    void setMissionCrate(bool on);

    void setPosition(float x, float y, float z) override; // actor vtable slot +0x48
    void setPosition(const Vector &v); // unpacks and dispatches slot +0x48
    void setRotate(int v);

    void setShootError(int v);

    void setShipGroup(AEGeometry *geom, int group, bool flag) override; // actor vtable slot +0x8
    void setSpeed(float v) override;

    void setWingmanCommand(int cmd, KIPlayer *target) override;

    void update(int dt) override;
};
#endif
