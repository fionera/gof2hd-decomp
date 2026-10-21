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
    AEGeometry *turretGeometry;  // turret barrel (yaw/pitch)
    AEGeometry *turretRoot;
    AEGeometry *turretExtraGeo;
    AEGeometry *shieldGeometry;  // supernova heat shield
    uint8_t shieldVisible;
    float shieldRadius;

    AbyssEngine::AEMath::Matrix field_0x40;  // 0x40..0x7c (ctor calls Matrix::Matrix() on this+0x40)
    float rocketPitch;                       // 0x7c (up/down in rocket control)
    float rocketYaw;                         // 0x80 (left/right in rocket control)
    uint8_t field_0x84;
    TargetFollowCamera *targetFollowCamera;

    Explosion *explosion;
    Explosion *explosion2;
    AbyssEngine::AEMath::Vector lastCrosshairPos;      // 0x94
    AbyssEngine::AEMath::Vector lastCrosshairShootPos; // 0xa0
    int field_0xac;
    uint8_t field_0xb0;
    uint8_t switchToStandardCam;
    uint8_t field_0xb2;
    int engineSoundId;
    float speed;
    float thrust;
    uint8_t _pad_0xc0[4]; // original field_0xc0 region (1-byte field + 3 pad)
    uint8_t freeLookMode;
    int boostSpeed;
    int boostTime;
    int boostDelay;
    int boostSoundId;
    int field_0xd8;
    AEGeometry *turretBaseGeo;
    AbyssEngine::AEMath::Vector moveDirection; // 0xe0
    AbyssEngine::AEMath::Vector moveTarget;    // 0xec
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
    // 64-bit tick counters (update() adds the frame time with adc); they give the class its 8-byte
    // alignment (sizeof == 0x3a0, the original operator new size).
    int64_t shieldRegenTick;
    int64_t repairHPTick;
    int64_t repairArmorTick;
    int lastHP;
    int shakeIntensity;
    int boostTimer;
    uint8_t boostingFlag;
    int field_0x140;
    uint8_t collide;
    uint8_t field_0x145;
    uint8_t field_0x146;
    AbyssEngine::AEMath::Vector dockOffsetVec;
    float handling;
    bool autoPilot;
    int autoPilotTarget;
    uint8_t goingToWaypointFlag;
    AbyssEngine::AEMath::Vector headingVec;
    uint8_t turretMode;
    uint32_t turretCamera;
    AEGeometry *dockCameraNode;
    AEGeometry *dockCameraLeaf;
    bool autoTurretEquipped;
    int autoTurretTimer;
    int autoTurretFireTimer;
    KIPlayer *autoTurretTarget;
    KIPlayer *autoTurretPrevTarget;
    int rocketControlGun;
    float rocketBanking;
    AEGeometry *dockCameraMid;
    bool turretActive;
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

    AbyssEngine::AEMath::Vector miningUpVec;
    uint8_t field_0x1d4;
    int dockScaling;
    int miningSettleTimer;
    KIPlayer *dockStation;
    int miningGame;
    int hackingGame;
    uint8_t dockingToStream;
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
    float shieldRegenRate;
    int field_0x21c;
    Hud *hud;
    AbyssEngine::AEMath::Vector turretOffsetVec;
    int field_0x230;
    uint8_t collidesWithStationFlag;
    uint8_t hardCoreMode;
    int hpGaugeImage;
    int dockArrowImage;
    int radarBlipImage1;
    int radarBlipImage2;
    int radarBlipImage3;
    int shakeAccum;
    int freeCargoSpace;
    float agility;
    float pitchRamp;
    float yawRamp;
    int field_0x260;
    int field_0x264;
    float rollAccum;
    float field_0x26c;
    float yawAccumD;
    float field_0x274;
    float pitchAccumD;
    float yawAccumF;
    int field_0x280;
    float maxBank;    // 0x284
    float bankFactor; // 0x288
    int field_0x28c;
    float bankHistory[5];   // 0x290
    int bankHistoryIndex;   // 0x2a4
    bool bankHistoryFilled; // 0x2a8
    uint8_t rollDirection;
    AbyssEngine::AEMath::Matrix rollMatrix; // 0x2ac (bank applied to the cockpit view)
    float rotateX;
    float rotateY;
    float rotateZ;
    uint8_t rolling;
    uint8_t field_0x2f5;
    int explosionTimer;
    int currentSystem;
    int smokeSystem;
    int explosionSmoke;
    uint8_t field_0x308;
    uint8_t field_0x309;
    uint8_t field_0x30a;
    int emergencySystemTimer;
    int emergencyVal1;
    AbyssEngine::AEMath::Vector emergencyVec;
    float emergencyVal2;
    uint8_t autoLevel;
    uint8_t field_0x325;
    int hitShakeTimer;
    uint8_t hitShake;
    uint8_t field_0x32d;
    uint8_t visible;
    uint8_t exhaustVisible;
    uint8_t aboutToReachAutoTargetFlag;
    int maneuverType;
    AbyssEngine::AEMath::Vector strafeTargetVec;
    AbyssEngine::AEMath::Vector facingVec;
    int maneuverTimer;
    // ASM: setAutoTurret writes autoTurretEnabled at [this,#853]=0x355, dockedFlag readers load @0x356.
    // The @0x354 slot is a distinct field; autoTurretEnabled is @0x355 and dockedFlag @0x356.
    uint8_t field_0x354;
    bool autoTurretEnabled;
    uint8_t dockedFlag;
    AbyssEngine::EaseInOutMatrix *easeMatrix;
    int field_0x35c;
    int dockTotalAmount;
    int dockTransferedAmount;
    int cloakRechargeMax;
    int spacePoint;
    uint8_t throttleStarted;
    // 3 bytes padding to align throttle to 4
    int throttle;
    uint8_t field_0x378; // bool flag (initialized as clz(ptr)>>5 in ctor)
    uint8_t _pad_0x379[3];
    float strafeAccel;          // 0x37c
    float strafeFactor;         // 0x380
    int field_0x384;            // 0x384
    uint32_t cloakMaterial1;    // 0x388
    uint32_t cloakMaterial2;    // 0x38c
    uint32_t cloakMaterial3;    // 0x390
    int originalBlendMode;
    uint8_t volatileGoods;
    uint8_t field_0x399;
    uint8_t field_0x39a;
    uint8_t lostMiningGameFlag;

    PlayerEgo(Player *player);

    ~PlayerEgo() noexcept;

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

    int approachAsteroid(Hud *hud, int unused, Radar *radar);

    bool approachDockingPoint(Hud *hud, int /*hud2*/, Radar *radar);

    bool autoTurretIsEnabled();

    void boost();

    unsigned char boosting();

    void calcCollision(Array<KIPlayer *> *candidates);

    void changeThrust(float v);

    void checkForTurret();

    unsigned char collidesWithStation();

    void deleteHackingGame();

    void dockToAsteroid(KIPlayer *kip, Radar *radar);

    void dockToDockingPoint(KIPlayer *station, Radar *radar);

    void dockToPlanet();

    void dockToStream(bool param);

    void down(int frameTime, float delta);

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

    Hud *getHUD();

    int getHackingGameDockIndex();

    float getHandling();

    int getHitpoints();

    int getHullDamageRate();

    Vec3 getPosition();

    float getRocketBanking();

    int getRoute();

    int getShieldDamageRate();

    int getSpeed();

    TargetFollowCamera *getTargetFollowCamera();

    Vec3 getTurretPosition();

    float getThrust();

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

    bool hasAutoTurret();

    bool hasCloak();

    unsigned char hasVolatileGoods();

    void hideShipForFirstPersonCameraView(bool param);

    void hitCamera();

    void initManeuver(int type);

    bool isAutoPilot();

    int isBoostRefreshed();

    unsigned char isChargingCloak();

    unsigned char isChargingDrive();

    unsigned char isCloaked();

    bool isDead();

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

    bool killLiberator();

    int levelCollision();

    void left(int frameTime, float delta);

    unsigned char lostMiningGame();

    float moveToPosition(AbyssEngine::AEMath::Vector target, bool steer, float speed);

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

    void right(int frameTime, float delta);

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

    bool setTurretMode(bool enable);

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

    bool toggleCloaking();

    int tryToStartEmergencySystem();

    void turnHorizontal(int a, float v);

    void turnVertical(int a, float v);

    void up(int frameTime, float delta);

    void update(int dt, Radar *radar, Hud *hud, Radio *radio, LevelScript *script, int arg5, bool mouseLocked,
                int controlMode);

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
static_assert(__builtin_offsetof(PlayerEgo, targetFollowCamera) == 0x88, "PlayerEgo::targetFollowCamera offset");

// Former-drift fields: each now lands at the offset encoded in its name (Ghidra-verified).
static_assert(__builtin_offsetof(PlayerEgo, rocketYaw) == 0x80, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xac) == 0xac, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xb0) == 0xb0, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0xb2) == 0xb2, "");
static_assert(__builtin_offsetof(PlayerEgo, boostTime) == 0xcc, "");
static_assert(__builtin_offsetof(PlayerEgo, boostDelay) == 0xd0, "");
static_assert(__builtin_offsetof(PlayerEgo, boostSoundId) == 0xd4, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x145) == 0x145, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x146) == 0x146, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x1a1) == 0x1a1, "");
static_assert(__builtin_offsetof(PlayerEgo, miningUpVec) == 0x1c8, "");
static_assert(__builtin_offsetof(PlayerEgo, bankHistoryIndex) == 0x2a4, "");
static_assert(__builtin_offsetof(PlayerEgo, bankHistoryFilled) == 0x2a8, "");
static_assert(__builtin_offsetof(PlayerEgo, collide) == 0x144, "");
static_assert(__builtin_offsetof(PlayerEgo, dockScaling) == 0x1d8, "");
static_assert(__builtin_offsetof(PlayerEgo, dockingToPlanet) == 0x1ee, "");
static_assert(__builtin_offsetof(PlayerEgo, hud) == 0x220, "");
static_assert(__builtin_offsetof(PlayerEgo, hardCoreMode) == 0x235, "");
static_assert(__builtin_offsetof(PlayerEgo, shakeAccum) == 0x24c, "");
static_assert(__builtin_offsetof(PlayerEgo, yawAccumF) == 0x27c, "");
static_assert(__builtin_offsetof(PlayerEgo, rotateX) == 0x2e8, "");
static_assert(__builtin_offsetof(PlayerEgo, rolling) == 0x2f4, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x2f5) == 0x2f5, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x309) == 0x309, "");
static_assert(__builtin_offsetof(PlayerEgo, field_0x32d) == 0x32d, "");
static_assert(__builtin_offsetof(PlayerEgo, originalBlendMode) == 0x394, "");
// Relocated matrices and the field immediately after the last drift field, to pin the tail.
static_assert(__builtin_offsetof(PlayerEgo, field_0x40) == 0x40, "");
static_assert(__builtin_offsetof(PlayerEgo, rollMatrix) == 0x2ac, "");
static_assert(__builtin_offsetof(PlayerEgo, volatileGoods) == 0x398, "");
static_assert(sizeof(PlayerEgo) == 0x3a0, "");
// (sizeof==924 assert dropped: it was host-clang-calibrated and rounds differently under the NDK
// ARM32 EABI; the per-field offset asserts above are the authoritative layout locks.)
#endif

#endif
