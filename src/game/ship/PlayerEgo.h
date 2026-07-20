#ifndef GOF2_PLAYEREGO_H
#define GOF2_PLAYEREGO_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "TargetFollowCamera.h"
#include "engine/math/Vector.h"
#include "engine/math/Matrix.h"
#include "engine/render/AEGeometry.h"
#include "game/weapons/Radar.h"
#include "game/weapons/RepairBeam.h"

#include "engine/math/AEMath.h"

#include "game/ship/ExplosionEmitterHolder.h"

#include "game/ship/MiningInputFlags.h"
class Player;
class Radar;

class AEGeometry;
class Explosion;
class Gun;
class Hud;
class Item;
class KIPlayer;
class Level;
class LevelScript;
class Radio;
class RepairBeam;
class Route;
class SpacePoint;
class TargetFollowCamera;
class TractorBeam;

namespace AbyssEngine {
    class EaseInOutMatrix;
}

typedef AbyssEngine::AEMath::Vector Vec3;

class PlayerEgo {
public:
    Player *player;

    // Former overlay region (offset 0x4..0x4c). These named pointers are the real fields.
    //   - The `transform` Matrix view aliases this storage at offset 0x4:
    //       reinterpret_cast<AbyssEngine::AEMath::Matrix &>(field_0x4)
    //   - The `rocketReturnMatrix` Matrix view aliases this storage at offset 0x10 (levelScript):
    //       reinterpret_cast<AbyssEngine::AEMath::Matrix &>(levelScript)
    AEGeometry *field_0x4;
    AEGeometry *geometry;
    Level *level;
    LevelScript *levelScript;
    Radar *field_0x14;
    Radio *radioRef;
    int field_0x1c;
    int field_0x20;
    uint8_t freeze;
    uint8_t inWormhole;
    AEGeometry *turretGeometry;
    AEGeometry *field_0x2c;
    AEGeometry *field_0x30;
    AEGeometry *gunYawGeo;
    AEGeometry *gunMuzzleRoot;
    AEGeometry *gunExtraGeo;

    float maneuverParam;
    // rollMatrix relocated here: it fills the original matrix region at 0x44..0x80
    // (AEMath::Matrix is float[15] = 0x3c, ending exactly at field_0x80 = 0x80).
    AbyssEngine::AEMath::Matrix rollMatrix;
    float field_0x80;
    uint8_t _pad_0x84[4]; // original field_0x84 region (1-byte field + 3 pad)
    int targetFollowCamera;

    Explosion *explosion;
    Explosion *explosion2;
    uint8_t _pad_0x94[24]; // missing fields between explosion2 and field_0xac (0x94..0xac)
    int field_0xac;
    uint8_t field_0xb0;
    uint8_t switchToStandardCam;
    uint8_t field_0xb2;
    int engineSoundId;
    int speed;
    float thrust;
    uint8_t _pad_0xc0[4]; // original field_0xc0 region (1-byte field + 3 pad)
    uint8_t freeLookMode;
    int boostSpeed;
    int field_0xcc;
    int field_0xd0;
    int boostDelay; // relocated from its drifted slot; real offset 0xd4 (used in PlayerEgo.cpp)
    void *boostSoundId; // lint: void_ptr  opaque FMOD sound-event id stored in a pointer-width slot (used only as (int)(intptr_t)); no typed pointee
    // lint: void_ptr  opaque FMOD sound-event id stored in a pointer-width slot (used only as (int)(intptr_t)); no typed pointee
    AEGeometry *rollGeometry;
    float rotX;
    float rotY;
    float rotZ;
    float waypointX;
    float waypointY;
    AEGeometry *gunBaseGeo;
    // Ghidra: movingStars@0xf8 sits between gunBaseGeo@0xf4 and route@0xfc. Our decomp lacked it; the
    // route..collide block was 4 low. Added here and the _pad_0x126 below shrinks by 4 to keep the
    // downstream asserts (field_0x145@0x145) pinned.
    int movingStars;
    int route;
    int pitchAccumDir;
    int yawAccumDir;
    // Ghidra: a 4-byte field@0x108 precedes currentSecondaryWeaponIndex@0x10c; ours lacked it.
    int field_0x108;
    int currentSecondaryWeaponIndex;
    float yawAccumulator;
    float pitchAccumulator;
    float yawRate;
    float pitchRate;
    // Ghidra: 4 fields (0x120/0x124/0x128/0x12c) precede lastHP(=combinedHp)@0x130; ours lacked them.
    int field_0x120;
    int field_0x124;
    int field_0x128;
    int field_0x12c;
    int lastHP;
    int shakeIntensity;
    int boostTimer;
    uint8_t boostingFlag;
    uint8_t collide;
    uint8_t _pad_0x13e[7]; // shrunk for movingStars + field_0x108 + 4 fields @0x120
    uint8_t field_0x145;
    uint8_t field_0x146;
    AbyssEngine::AEMath::Vector dockOffsetVec;
    int handling;
    uint8_t autoPilot;
    int autoPilotTarget;
    uint8_t goingToWaypointFlag;
    AbyssEngine::AEMath::Vector headingVec;
    uint8_t turretMode;
    uint32_t turretCamera;
    AEGeometry *dockCameraNode;
    AEGeometry *dockCameraLeaf;
    uint8_t autoTurretEquipped;
    int autoTurretTimer;
    int autoTurretFireTimer;
    KIPlayer *autoTurretTarget;
    KIPlayer *autoTurretPrevTarget;
    int rocketControlGun;
    int rocketBanking;
    AEGeometry *dockCameraMid;
    uint8_t turretActive;
    uint8_t field_0x1a1;
    float lookPitch;
    float lookYaw;
    uint8_t cloaked;
    uint8_t chargingCloak;
    Item *cloak;
    TractorBeam *tractorBeam;
    Array<RepairBeam *> *repairBeams;
    int asteroidTarget;
    int dockingState;
    int dockingPointIndex;

    // `resumeFlag` is the low byte of this int slot:
    //   reinterpret_cast<uint8_t &>(dockApproachDist)
    int dockApproachDist;

    int dockApproachThreshold;
    int field_0x1d0;
    int dockScaling;
    int miningSettleTimer;
    KIPlayer *dockStation;
    // miningGame is at 0x1e4 (drift: our 0x1e0->orig 0x1e4); field_0x1e0@0x1e0 precedes it. The +4 from
    // these two is balanced by removing the spurious _pad_0x2a0[4] (original matrix region is 0x3c),
    // keeping field_0x2a4@0x2a4; turretHudMatrix shifts 0x264->0x268. (Doubles above are now floats.)
    int field_0x1e0;
    int miningGame;
    int hackingGame;
    short docked;
    uint8_t dockedToStream;
    uint8_t dockingToPlanet;
    int planetDockTimer;
    uint8_t computerControlled;
    int turretPitch;
    int driveCharge;
    int driveChargeMax;
    uint8_t chargingDrive;
    int cloakCharge;
    int cloakRechargeTimer;
    int cloakChargeMax;
    int cloakDischargeMax;
    int hud;
    AbyssEngine::AEMath::Vector turretOffsetVec;
    uint8_t collidesWithStationFlag;
    uint8_t hardCoreMode;
    int hpGaugeImage;
    int dockArrowImage;
    int radarBlipImage1;
    int radarBlipImage2;
    int radarBlipImage3;
    int shakeAccum;
    float pitchRamp;
    float yawRamp;
    // ASM ground truth: these were modeled as 8-byte doubles but the .cpp accesses them via (float&),
    // and Ghidra shows separate 4-byte fields here (freeCargoSpace@0x250/flAgility@0x254 etc.). Split
    // into floats + their separated high-half fields (size-neutral 24 bytes, removes 8-byte alignment).
    float rollAccum;
    int field_0x250;
    float yawAccumD;
    int field_0x258;
    float pitchAccumD;
    int field_0x260;
    float yawAccumF;
    // turretHudMatrix relocated here: it fills the original matrix region at 0x264..0x2a4.
    AbyssEngine::AEMath::Matrix turretHudMatrix;
    int field_0x2a4;
    uint8_t field_0x2a8;
    uint8_t rollDirection;
    uint8_t _pad_0x2aa[20]; // region vacated by relocated rollMatrix + missing fields (0x2aa..0x2c0)
    void *field_0x2c0; // lint: void_ptr  opaque flag, only ever compared != 0; never assigned a typed value in the tree
    uint8_t gunExtraVisible;
    float rotateX;
    float rotateY;
    float rotateZ;
    uint8_t _pad_before_rolling[32]; // ASM (roll): rolling is at 0x2f4, AFTER this gap, not before it
    uint8_t rolling;
    uint8_t field_0x2f5;
    int explosionTimer;
    int currentSystem;
    int smokeSystem;
    int explosionSmoke;
    uint8_t _pad_0x308[1]; // original field_0x308
    uint8_t field_0x309;
    int emergencySystemTimer;
    int emergencyVal1;
    AbyssEngine::AEMath::Vector emergencyVec;
    float emergencyVal2;
    uint8_t autoLevel;
    int hitShakeTimer;
    uint8_t hitShake;
    uint8_t field_0x32d;
    uint8_t visible;
    uint8_t exhaustVisible;
    uint8_t aboutToReachAutoTargetFlag;
    int maneuverType;
    AbyssEngine::AEMath::Vector strafeTargetVec;
    AbyssEngine::AEMath::Vector facingVec;
    SpacePoint *navPoint;
    // ASM: setAutoTurret writes autoTurretEnabled at [this,#853]=0x355, dockedFlag readers load @0x356.
    // The @0x354 slot is a distinct field; autoTurretEnabled is @0x355 and dockedFlag @0x356.
    uint8_t field_0x354;
    uint8_t autoTurretEnabled;
    uint8_t dockedFlag;
    AbyssEngine::EaseInOutMatrix *easeMatrix;
    int dockTotalAmount;
    int dockTransferedAmount;
    // ASM: getCloakRechargeRate reads cloakRechargeMax at [this,#872]=0x368; ours had it @0x364.
    // A 4-byte field precedes it; compensated by shrinking _pad_0x388 below so field_0x394 stays pinned.
    int field_0x364;
    int cloakRechargeMax;
    int spacePoint;
    uint8_t throttleStarted;
    int throttle;
    float strafeAccel;
    SpacePoint *strafeNavPoint;
    int cloakMaterial1;
    int cloakMaterial2;
    int cloakMaterial3;
    uint8_t _pad_0x38c[8]; // shrunk 12->8 to absorb field_0x364 above; field_0x394 stays pinned
    unsigned char *field_0x394;
    uint8_t volatileGoods;
    uint8_t lostMiningGameFlag;

    PlayerEgo(Player *player);

    ~PlayerEgo() noexcept(false);

    Vec3 GetDirVector();

    Vec3 GetUpVector();

    void PauseEngineSound();

    void PlayEngineSound();

    void ResumeEngineSound();

    void StopEngineSound();

    unsigned char aboutToReachAutoTarget();

    void addGun(Gun *gun, int x);

    void addGun(Array<Gun *> *arr, int x);

    void addNukeVolatileForce(float v);

    void alignToHorizon();

    void approachAsteroid(Hud *hud, int hud2, Radar *radar);

    int approachDockingPoint(Hud *hud, int /*hud2*/, Radar *radar);

    unsigned char autoTurretIsEnabled();

    void boost();

    unsigned char boosting();

    void calcCollision(Array<KIPlayer *> *candidates);

    void changeThrust(float v);

    void checkForTurret();

    unsigned char collidesWithStation();

    void deleteHackingGame();

    void dockToAsteroid(KIPlayer *kip, Radar *radar);

    void dockToDockingPoint(KIPlayer *kip, Radar *radar);

    void dockToPlanet();

    void dockToStream(bool param);

    float down(int frameTime, float delta);

    void draw(bool allowHud);

    void drawThrottle();

    bool driveReady();

    bool emergencySystemActive();

    void endExplosion();

    void explode();

    bool explosionEnded();

    void forceBoost();

    int getAutoPilotTarget();

    float getBoostPercentage();

    float getBoostRate();

    int getBoostSpeed();

    float getCloakRate();

    float getCloakRechargeRate();

    float getCloakingPercentage();

    int getCurrentMiningAmount();

    int getCurrentSecondaryWeaponIndex();

    int getDockTotalAmount();

    int getDockTransferedAmount();

    float getDriveChargeRate();

    int getHUD();

    int getHackingGameDockIndex();

    int getHandling();

    int getHitpoints();

    int getHullDamageRate();

    Vec3 getPosition();

    int getRocketBanking();

    int getRoute();

    int getShieldDamageRate();

    int getSpeed();

    int getTargetFollowCamera();

    Vec3 getTurretPosition();

    int getThrust();

    float getVolatileForce();

    bool goingToPlanet();

    int goingToStation();

    int goingToStream();

    unsigned char goingToWaypoint();

    bool goingToWormhole();

    void hackingRotateLCW();

    void hackingRotateRCW();

    void hackingShuffle();

    int hackingWon();

    void handleAutoTurret(int dt);

    void handleShip(int dt);

    void handleTurretView(int dt);

    unsigned char hasAutoTurret();

    bool hasCloak();

    unsigned char hasVolatileGoods();

    void hideShipForFirstPersonCameraView(bool param);

    void hitCamera();

    void initManeuver(int type);

    unsigned char isAutoPilot();

    int isBoostRefreshed();

    unsigned char isChargingCloak();

    unsigned char isChargingDrive();

    unsigned char isCloaked();

    int isDead();

    bool isDockedToAsteroid();

    bool isDockedToDockingPoint();

    bool isDockedToMiningPlant();

    bool isDockedToPlanet();

    unsigned char isDockedToStream();

    bool isDockingToAsteroid();

    bool isDockingToDockingPoint();

    unsigned char isDockingToPlanet();

    unsigned char isDockingToStream();

    bool isHacking();

    bool isInDockingProcedure();

    unsigned char isInFreeLookMode();

    bool isInRocketControl();

    unsigned char isInTurretMode();

    bool isInWormhole();

    bool isLandingOrTakingOff();

    bool isMining();

    bool isRechargingCloak();

    void killLiberator();

    int levelCollision();

    float left(int frameTime, float delta);

    unsigned char lostMiningGame();

    void moveToPosition(AbyssEngine::AEMath::Vector target, bool steer, float speed);

    void pitchAllPrimaryGuns(float);

    bool readyForCloak();

    bool readyToBoost();

    void refillGunDelay();

    void removeRoute();

    void render(bool allowHud);

    void resetChargingDrive();

    void resetGunDelay();

    void resetLastHP();

    void resetMovement();

    void revive();

    float right(int frameTime, float delta);

    void roll(int amount);

    void rotate(float rx, float ry, float rz);

    void setActive(bool);

    void setAutoPilot(KIPlayer *kip);

    void setAutoTurret(bool on);

    void setCollide(bool v);

    void setComputerControlled(bool v);

    void setCurrentSecondaryWeaponIndex(int idx);

    void setDockingCamera();

    void setDockingState(int s);

    void setExhaustVisible(bool param);

    void setFreeLookMode(bool v);

    void setFreeze(bool v);

    void setLevel(Level *level);

    void setPosition(AbyssEngine::AEMath::Vector v);

    void setPosition(float x, float y, float z);

    void setRocketControl(Gun *gun, AEGeometry *geo);

    void setRotation(float rx, float ry, float rz);

    void setRoute(Route *v);

    void setShip(int race, int group);

    void setSpeed(float v);

    void setTargetFollowCamera(TargetFollowCamera *cam);

    void setThrust(float v);

    void setTurretMode(bool enable);

    void setTurretPosition(AbyssEngine::AEMath::Vector v);

    void setVisible(bool v);

    void shake(int amount);

    void shoot(int weapon, int type);

    int shouldSwitchToFreeLookCam();

    int shouldSwitchToStandardCam();

    void startJumpDrive();

    void startSmokeEmission();

    void stopBoost();

    void stopMining();

    void stopPlanetDock();

    void stopShooting(int param);

    void strafe(int /*dir*/, bool positive);

    void throttleChanged();

    void toggleCloaking();

    int tryToStartEmergencySystem();

    void turnHorizontal(int a, float v);

    void turnVertical(int a, float v);

    float up(int frameTime, float delta);

    void update(int dt, Radar *radar, Hud *hud, Radio *radio, LevelScript *script, int arg5, bool arg6, int arg7);

    int updateManeuver();

    static AbyssEngine::AEMath::Vector crosshairPos;
    static AbyssEngine::AEMath::Vector crosshairShootPos;
    static AbyssEngine::AEMath::Vector vec_up;
};

#if __SIZEOF_POINTER__ == 4
// field_0x4 is the storage the former `transform` Matrix view aliased (offset 0x4).
static_assert(__builtin_offsetof(PlayerEgo, field_0x4) == 0x4, "PlayerEgo::transform overlay offset");
// levelScript is the storage the former `rocketReturnMatrix` Matrix view aliased (offset 0x10).
static_assert(__builtin_offsetof(PlayerEgo, levelScript) == 0x10, "PlayerEgo::rocketReturnMatrix overlay offset");
static_assert(__builtin_offsetof(PlayerEgo, maneuverParam) == 0x40, "PlayerEgo::maneuverParam offset");
static_assert(__builtin_offsetof(PlayerEgo, targetFollowCamera) == 0x88, "PlayerEgo::targetFollowCamera offset");
// resumeFlag was the low byte of this 4-byte int slot.
static_assert(__builtin_offsetof(PlayerEgo, dockApproachThreshold) -
              __builtin_offsetof(PlayerEgo, dockApproachDist) == 4,
              "PlayerEgo::dockApproachDist overlay slot width");

// Former-drift fields: each now lands at the offset encoded in its name (Ghidra-verified).
static_assert(__builtin_offsetof(PlayerEgo, field_0x80) == 0x80, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xac) == 0xac, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xb0) == 0xb0, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xb2) == 0xb2, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xcc) == 0xcc, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xd0) == 0xd0, "");
static_assert(__builtin_offsetof(PlayerEgo, boostDelay) == 0xd4, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x145) == 0x145, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x146) == 0x146, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x1a1) == 0x1a1, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x1d0) == 0x1d0, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x2a4) == 0x2a4, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x2a8) == 0x2a8, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x2c0) == 0x2c0, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x2f5) == 0x2f5, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x309) == 0x309, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x32d) == 0x32d, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x394) == 0x394, "");
// Relocated matrices and the field immediately after the last drift field, to pin the tail.
static_assert(__builtin_offsetof(PlayerEgo, rollMatrix) == 0x44, "");
static_assert(__builtin_offsetof(PlayerEgo, turretHudMatrix) == 0x268, "");
static_assert(__builtin_offsetof(PlayerEgo, volatileGoods) == 0x398, "");
// (sizeof==924 assert dropped: it was host-clang-calibrated and rounds differently under the NDK
// ARM32 EABI; the per-field offset asserts above are the authoritative layout locks.)
#endif

#endif
