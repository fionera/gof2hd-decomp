#ifndef GOF2_PLAYERFIGHTER_H
#define GOF2_PLAYERFIGHTER_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "engine/math/EaseInOutMatrix.h"

#include "engine/math/Matrix.h"
#include "engine/render/Trail.h"
#include "game/mission/Explosion.h"
#include "game/ship/KIPlayer.h"

#include "engine/math/Vector.h"

class Player;

class AEGeometry;
class BoundingVolume;
class Explosion;
class KIPlayer;
class Level;
class Route;
class SpacePoint;
class Trail;

namespace AbyssEngine {
    class EaseInOutMatrix;
}

class PlayerFighter : public KIPlayer {
public:
    // Views of KIPlayer storage that PlayerFighter uses with its own meaning (offsets are the
    // KIPlayer ones). KIPlayer::shipGroup (0x28) is the fighter's race; KIPlayer::field_0xe4 is the
    // current wingman command and KIPlayer::wingmanCommand (0xe0) the wingman's formation slot.
    signed char &field_0xdc() { return reinterpret_cast<signed char &>(this->wingmanFlag); }
    Vector &renderPosition() { return reinterpret_cast<Vector &>(this->autoPilotState); } // 0x2c: position
    signed char &field_0x43() { return reinterpret_cast<signed char &>(this->reviveLockFlag); }
    uint8_t &crateCaptured() { return reinterpret_cast<uint8_t &>(this->hasCargo); }
    Array<int> *&lootList() { return this->cargo; }
    uint8_t &missionCrateLost() { return this->diedWithMissionCrate; }
    uint8_t &missionCrateCaptured() { return this->lostMissionCrateToEgo; }
    uint8_t &crateLost() { return this->field_0x6a; }
    Route *&routeClone() { return this->KIPlayer::route; }
    uint8_t &exhaustHidden() { return this->field_0x74; }
    AEGeometry *&wreckGeometry() { return this->crateGeometry; }
    Vector &tempVector() { return reinterpret_cast<Vector &>(this->field_0x90); }   // 0x90 scratch
    Vector &steerVector() { return reinterpret_cast<Vector &>(this->field_0x9c); }  // 0x9c scratch
    uint8_t &isMissionCrate() { return reinterpret_cast<uint8_t &>(this->carriesMissionCrate); }
    int32_t &deathTimer() { return this->field_0xd8; }
    uint8_t &field_0xf5() { return this->visibleFlag; }
    int32_t &fov() { return this->engineSoundEvent; }
    signed char &rollActive() { return reinterpret_cast<signed char &>(this->field_0x100); }
    int32_t &pushTimer() { return this->field_0x104; }
    int32_t &pushDuration() { return this->field_0x108; }
    Vector &pushNormal() { return reinterpret_cast<Vector &>(this->field_0x10c); }
    Vector &pushImpulse() { return reinterpret_cast<Vector &>(this->field_0x118); }
    Explosion *explosion;
    int32_t field_0x128; // sight range
    uint8_t field_0x12c; // following its route
    uint8_t field_0x12d; // stray from target (no steering)
    uint8_t targetIsActive;
    signed char field_0x12f;
    int32_t field_0x130;
    int32_t field_0x134;
    int32_t field_0x138;
    signed char field_0x13c;
    uint8_t field_0x13d;
    signed char field_0x13e;
    int32_t field_0x140; // active weapon slot (0/1)
    Route *defaultRoute;
    // Escort/attack target (`target__` in the Java lineage); its `field_5e` is the
    // hidden/sleep flag checked by the proximity-awaken and movement subsystems.
    Player *targetPlayer;
    Route *commandRoute;
    Array<BoundingVolume *> *boundingVolumes;
    Trail *trail;
    Vector workingPosition;
    Vector targetPos;
    Vector distanceToTarget;
    Vector field_0x17c;
    Vector dyingSpin;
    Vector dyingDirection;
    int32_t engineTrailSystem;
    float rotate;
    float shootError;
    float speed;
    int32_t field_0x1b0;
    int32_t boostProb;
    int32_t behaviourChangeTick;
    int32_t field_0x1bc;
    int32_t boostTick;
    int32_t boostTimeLimit;
    int32_t trailTick;
    int32_t field_0x1cc;
    long long frameTime; // frame time (64-bit, from update(dt))
    int32_t hitpoints;
    int32_t damageAccumulator;
    signed char damageEscape;
    int32_t initialWaypoint;
    float currentSpeed;
    float targetSpeed;
    float currentRotate;
    uint8_t boosting;
    uint8_t field_0x1f5;
    int32_t dyingTimer;
    signed char smoking;
    int32_t field_0x200;
    int32_t field_0x204;
    int32_t field_0x208;
    float targetRoll;
    float smoothRoll;
    int32_t field_0x214;
    AbyssEngine::Matrix easeBaseMatrix;
    // roll() strh.w [this,#0x254] writes this pair right where easeBaseMatrix ends;
    // rollMatrix follows at 0x258 after 2 pad bytes.
    signed char field_0x254;
    signed char field_0x255;
    AbyssEngine::Matrix rollMatrix;
    float maxRoll;
    float rollFactor;
    float rollSamples[5];   // ring buffer of angle samples (+0x29c..+0x2af, 5 floats)
    int32_t rollSampleIndex;
    uint8_t rollBufferFilled;
    uint8_t isShooting;   // 0x2b5: set to 1 after shoot(); cleared on target-lost or hidden
    uint8_t _pad_0x2b6[2];
    AbyssEngine::EaseInOutMatrix *easeMatrix;
    // field_0x2c9/field_0x2cd were Ghidra artifacts of the ctor's overlapping unaligned
    // zero-stores (str.w [+0x2c9], str.w [+0x2cd]) spanning cloakTimer..cloakActive.
    int32_t dockTimer;
    int32_t passengersMoved;
    SpacePoint *spacePoint;
    int32_t cloakTimer;
    int32_t cloakDuration;
    uint8_t cloakActive;
    int32_t cloakCooldown;
    uint8_t cloakingPossible;
    signed char field_0x2d9;
    uint32_t cloakMaterial;
    int32_t cloakSavedMode;
    uint8_t aiDisabled;
    int32_t gunSwitchTimer;

    PlayerFighter(int faction, int wingmanCmd, Player *player, AEGeometry *geom,
                  float x, float y, float z, bool active);

    ~PlayerFighter();

    void awake() override;

    void reset();

    void revive() override;

    void cloak(int dur, bool b);

    int collide(float x, float y, float z) override;

    void handleCloaking();

    uint8_t hasCrateCaptured();

    uint8_t hasCrateLost();

    uint8_t hasMissionCrateCaptured();

    uint8_t hasMissionCrateLost();

    void initPush(const Vector &target, int radius) override;

    int outerCollide(float x, float y, float z) override;

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

    void setPosition(float x, float y, float z) override;

    void setPosition(const Vector &v);

    void setRotate(int v);

    void setShootError(int v);

    void setShipGroup(AEGeometry *geom, int group, bool flag) override;

    void setSpeed(float v) override;

    void setWingmanCommand(int cmd, KIPlayer *target) override;

    void update(int dt) override;

    static int stationRouteAliens;
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(PlayerFighter, field_0x17c) == 0x17c, "PlayerFighter::field_0x17c offset");
static_assert(__builtin_offsetof(PlayerFighter, engineTrailSystem) == 0x1a0, "PlayerFighter::engineTrailSystem offset");
static_assert(__builtin_offsetof(PlayerFighter, field_0x1bc) == 0x1bc, "PlayerFighter::field_0x1bc offset");
static_assert(__builtin_offsetof(PlayerFighter, frameTime) == 0x1d0, "PlayerFighter::frameTime offset");
static_assert(__builtin_offsetof(PlayerFighter, hitpoints) == 0x1d8, "PlayerFighter::hitpoints offset");
static_assert(__builtin_offsetof(PlayerFighter, field_0x254) == 0x254, "PlayerFighter::field_0x254 offset");
static_assert(__builtin_offsetof(PlayerFighter, rollMatrix) == 0x258, "PlayerFighter::rollMatrix offset");
static_assert(__builtin_offsetof(PlayerFighter, dockTimer) == 0x2bc, "PlayerFighter::dockTimer offset");
static_assert(__builtin_offsetof(PlayerFighter, spacePoint) == 0x2c4, "PlayerFighter::spacePoint offset");
static_assert(__builtin_offsetof(PlayerFighter, cloakTimer) == 0x2c8, "PlayerFighter::cloakTimer offset");
static_assert(__builtin_offsetof(PlayerFighter, cloakDuration) == 0x2cc, "PlayerFighter::cloakDuration offset");
static_assert(__builtin_offsetof(PlayerFighter, cloakActive) == 0x2d0, "PlayerFighter::cloakActive offset");
static_assert(__builtin_offsetof(PlayerFighter, field_0x2d9) == 0x2d9, "PlayerFighter::field_0x2d9 offset");
#endif
#endif
