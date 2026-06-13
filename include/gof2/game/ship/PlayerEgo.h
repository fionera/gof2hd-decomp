#ifndef GOF2_PLAYEREGO_H
#define GOF2_PLAYEREGO_H
#include "gof2/common.h"
// Vec3 — the 3-component math vector returned by several PlayerEgo methods.
// Alias to the foundation Vector type (AbyssEngine::AEMath::Vector via common.h).
typedef Vector Vec3;
// PlayerEgo — the player's own ship. Top-level class (NO namespace).
// Layout recovered from the per-method byte-offset accessors and the ctor field
// init. Trailing comments are the original 32-bit offsets, kept for cross-
// reference; the live layout is natural 64-bit.

class RepairBeam;          // element type of the repairBeams array.

// Other game types referenced by pointer only — keep opaque.
// (Player/Ship/etc. forward-declared in fwd.h via common.h; Vector/Matrix come from math.h.)

// Byte-offset helper still used by other TUs (e.g. Hud.cpp) for cross-object access.
static inline char*           B(void* p, int off) { return (char*)p + off; }

class PlayerEgo {
public:
    void* player;                         // +0x0    wrapped Player (ship logic / HP / guns)
    AEGeometry* field_0x4;                // +0x4    (transform-local source geometry)
    AEGeometry* geometry;                 // +0x8    hull / ship root geometry
    Level* level;                         // +0xc
    LevelScript* levelScript;             // +0x10
    void* field_0x14;                     // +0x14   (KIPlayer-style handle)
    int field_0x1c;                       // +0x1c   geometry/camera handle
    uint8_t freeze;                       // +0x24   frozen / collide-off flag
    uint8_t inWormhole;                   // +0x25
    void* turretGeometry;                 // +0x28
    void* field_0x2c;                     // +0x2c   geometry node (freed in dtor)
    void* field_0x30;                     // +0x30   geometry node (freed in dtor)
    void* gunYawGeo;                      // +0x34   gun yaw (barrel) geometry
    void* gunMuzzleRoot;                  // +0x38   gun muzzle root geometry
    void* gunExtraGeo;                    // +0x3c   optional extra gun child geometry
    float maneuverParam;                  // +0x7c   scripted-maneuver pitch param
    float field_0x80;                     // +0x80
    int targetFollowCamera;               // +0x88   TargetFollowCamera handle
    void* explosion;                      // +0x8c
    void* explosion2;                     // +0x90
    int field_0xac;                       // +0xac   emergency-system effect geometry
    int field_0xb0;                       // +0xb0   (handling store)
    uint8_t switchToStandardCam;          // +0xb1   one-shot "switch to standard cam" flag
    uint8_t field_0xb2;                   // +0xb2
    int engineSoundId;                    // +0xb4   FMod sound handle for engine/gamma SFX
    int speed;                            // +0xb8
    float thrust;                         // +0xbc
    uint8_t freeLookMode;                 // +0xc4
    int boostSpeed;                       // +0xc8
    int field_0xcc;                       // +0xcc   (boost-percentage period base)
    int field_0xd0;                       // +0xd0   (boost-rate denominator)
    void* boostSoundId;                   // +0xd4   FMod sound handle for boost SFX
    void* rollGeometry;                   // +0xdc   roll/banking geometry node
    float rotX;                           // +0xe0   setPosition3 source X
    float rotY;                           // +0xe4   setPosition3 source Y
    float rotZ;                           // +0xe8   setPosition3 source Z
    float waypointX;                      // +0xec   steer-to-waypoint target X
    float waypointY;                      // +0xf0   steer-to-waypoint target Y
    void* gunBaseGeo;                     // +0xf4   gun base geometry (also waypoint-Z storage)
    int route;                            // +0xfc   Route*
    int pitchAccumDir;                    // +0x100  pitch direction flag (-1/+1)
    int yawAccumDir;                      // +0x104  yaw direction flag (-1/+1)
    int currentSecondaryWeaponIndex;      // +0x10c
    float yawAccumulator;                 // +0x110
    float pitchAccumulator;               // +0x114
    float yawRate;                        // +0x118
    float pitchRate;                      // +0x11c
    int lastHP;                           // +0x130
    int shakeIntensity;                   // +0x134
    int boostTimer;                       // +0x138
    uint8_t boostingFlag;                     // +0x13c
    uint8_t collide;                      // +0x144  collide-disable flag
    uint8_t field_0x145;                  // +0x145
    uint8_t field_0x146;                  // +0x146  (boost-enabled flag)
    Vector dockOffsetVec;                 // +0x148  dock-rig offset vector (x/y/z)
    int boostDelay;                       // +0x150
    int handling;                         // +0x154
    uint8_t autoPilot;                    // +0x158
    int autoPilotTarget;                  // +0x15c  current waypoint/landmark
    uint8_t goingToWaypointFlag;              // +0x160
    Vector headingVec;                    // +0x164  desired heading vector (moveToPosition)
    uint8_t turretMode;                   // +0x170  turret available/equipped flag
    uint32_t turretCamera;                // +0x174  turret/dock camera handle
    void* dockCameraNode;                 // +0x178  dock/turret camera node geometry (root)
    void* dockCameraLeaf;                 // +0x17c  dock/turret camera leaf node
    uint8_t autoTurretEquipped;                // +0x180
    int autoTurretTimer;                  // +0x184  rescan timer (every 3s)
    int autoTurretFireTimer;              // +0x188
    void* autoTurretTarget;               // +0x18c  current auto-turret target
    void* autoTurretPrevTarget;           // +0x190
    int rocketControlGun;                 // +0x194  rocket-control gun handle
    int rocketBanking;                    // +0x198
    void* dockCameraMid;                  // +0x19c  dock/turret camera mid node
    uint8_t turretActive;                 // +0x1a0  in-turret-mode flag
    uint8_t field_0x1a1;                  // +0x1a1
    float lookPitch;                      // +0x1a4  turret look pitch accumulator
    float lookYaw;                        // +0x1a8  turret look yaw accumulator
    uint8_t cloaked;                      // +0x1ac
    uint8_t chargingCloak;                // +0x1ad
    void* cloak;                          // +0x1b0  cloak device handle
    void* tractorBeam;                    // +0x1b4
    Array<RepairBeam*>* repairBeams;      // +0x1b8  active repair-beam effects
    int asteroidTarget;                   // +0x1bc  docked/approached asteroid
    int dockingState;                     // +0x1c0  docking-procedure flag
    int dockingPointIndex;                // +0x1c4  docking-point/landing state index
    int dockApproachDist;                 // +0x1c8
    int dockApproachThreshold;            // +0x1cc
    int field_0x1d0;                      // +0x1d0
    int dockScaling;                      // +0x1d8
    int miningSettleTimer;                // +0x1dc
    void* dockStation;                    // +0x1e0  station/target being approached
    int miningGame;                       // +0x1e4  MiningGame*
    int hackingGame;                      // +0x1e8  HackingGame*
    short docked;                         // +0x1ec  docked-to-* state word
    uint8_t dockedToStream;               // +0x1ed
    uint8_t dockingToPlanet;              // +0x1ee
    int planetDockTimer;                  // +0x1f0
    uint8_t computerControlled;           // +0x1f4
    int turretPitch;                      // +0x1f8
    int driveCharge;                      // +0x1fc  jump-drive charge timer
    int driveChargeMax;                   // +0x200
    uint8_t chargingDrive;                // +0x204
    int cloakCharge;                      // +0x208
    int cloakRechargeTimer;               // +0x20c
    int cloakChargeMax;                   // +0x210  cloak full-charge threshold
    int cloakDischargeMax;                // +0x214
    int hud;                              // +0x220  Hud handle
    Vector turretOffsetVec;               // +0x224  turret-rig offset vector (x/y/z)
    uint8_t collidesWithStationFlag;          // +0x234
    uint8_t field_0x235;                  // +0x235
    int hpGaugeImage;                     // +0x238  HP-gauge image handle
    int dockArrowImage;                   // +0x23c  dock-arrow image handle
    int radarBlipImage1;                  // +0x240
    int radarBlipImage2;                  // +0x244
    int radarBlipImage3;                  // +0x248
    int shakeAccum;                       // +0x24c  hit-shake accumulator timer
    float pitchRamp;                      // +0x258  pitch ramp rate
    float yawRamp;                        // +0x25c  yaw ramp rate
    double rollAccum;                     // +0x268
    double yawAccumD;                     // +0x270
    double pitchAccumD;                   // +0x278  pitch ramp accumulator
    float yawAccumF;                      // +0x27c  yaw ramp accumulator
    int field_0x2a4;                      // +0x2a4
    uint8_t field_0x2a8;                  // +0x2a8
    uint8_t rollDirection;                // +0x2a9  roll/auto-level direction (0/1/2)
    Matrix rollMatrix;                    // +0x2ac  embedded 60-byte roll matrix
    void* field_0x2c0;                    // +0x2c0
    uint8_t gunExtraVisible;              // +0x2c4  extra-gun visibility flag
    float rotateX;                        // +0x2e8  setRotation X
    float rotateY;                        // +0x2ec  setRotation Y
    float rotateZ;                        // +0x2f0  setRotation Z
    uint8_t rolling;                      // +0x2f4  roll-active flag
    uint8_t field_0x2f5;                  // +0x2f5
    int explosionTimer;                   // +0x2f8
    int currentSystem;                    // +0x2fc  current SolarSystem handle
    int smokeSystem;                      // +0x300  smoke particle system
    int explosionSmoke;                   // +0x304  explosion smoke particle system
    uint8_t field_0x309;                  // +0x309  (visible mirror)
    int emergencySystemTimer;             // +0x30c
    int emergencyVal1;                    // +0x310  cached emergency-system value
    Vector emergencyVec;                  // +0x314  cached emergency transform vector (0x314..0x31c)
    float emergencyVal2;                  // +0x320
    uint8_t autoLevel;                    // +0x324  auto-level (horizon-align) flag
    int hitShakeTimer;                    // +0x328
    uint8_t hitShake;                     // +0x32c  hit-shake active flag
    uint8_t field_0x32d;                  // +0x32d
    uint8_t visible;                      // +0x32e
    uint8_t exhaustVisible;               // +0x32f
    uint8_t aboutToReachAutoTargetFlag;       // +0x330
    int maneuverType;                     // +0x334  scripted-maneuver type
    Vector strafeTargetVec;               // +0x338  strafe-run target vector (x/y/z @ 0x338..0x340)
    Vector facingVec;                     // +0x344  current facing direction vector
    void* navPoint;                       // +0x350  nearest navigation point
    uint8_t autoTurretEnabled;            // +0x355
    uint8_t dockedFlag;                   // +0x356  docked / in-station flag
    void* easeMatrix;                     // +0x358  EaseInOutMatrix*
    int dockTotalAmount;                  // +0x360
    int dockTransferedAmount;             // +0x364
    int cloakRechargeMax;                 // +0x368
    int spacePoint;                       // +0x36c  SpacePoint handle (route navigation)
    uint8_t throttleStarted;              // +0x370
    int throttle;                         // +0x374
    float strafeAccel;                    // +0x37c  lateral strafe acceleration
    void* strafeNavPoint;                 // +0x380  nav SpacePoint (also strafe-target storage)
    int cloakMaterial1;                   // +0x388  cloak material handle
    int cloakMaterial2;                   // +0x38c
    int cloakMaterial3;                   // +0x390
    void* field_0x394;                    // +0x394  mesh-derived handle
    uint8_t volatileGoods;             // +0x398
    uint8_t lostMiningGameFlag;               // +0x39b
    Matrix turretHudMatrix;               // +0x4c8  embedded 60-byte turret/HUD matrix

    // Real C++ ctor/dtor so the demangled symbols match the binary.
    PlayerEgo(Player* player);
    ~PlayerEgo() noexcept(false);

    // ---- methods (converted from free functions) ----
    Vec3 GetDirVector();
    Vec3 GetUpVector();
    void PauseEngineSound();
    void PlayEngineSound();
    void ResumeEngineSound();
    void StopEngineSound();
    unsigned char aboutToReachAutoTarget();
    void addGun(void* gun, int x);
    void addGun2(void* arr, int x);
    void addNukeVolatileForce(float v);
    void alignToHorizon();
    void approachAsteroid(int hud2, void *radar);
    int approachDockingPoint(void *hud, int /*hud2*/, void *radar);
    unsigned char autoTurretIsEnabled();
    void boost();
    unsigned char boosting();
    void calcCollision(void *candidates);
    void changeThrust(float v);
    void checkForTurret();
    unsigned char collidesWithStation();
    void deleteHackingGame();
    void dockToAsteroid(void *radar);
    void dockToDockingPoint(void *radar);
    void dockToPlanet();
    void dockToStream(bool param);
    float down(int frameTime, float delta);
    void draw(int allowHud);
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
    float left(int frameTime, float delta);
    unsigned char lostMiningGame();
    void moveToPosition(float tx, float ty, float tz, int steer, float speed);
    void pitchAllPrimaryGuns(float);
    bool readyForCloak();
    bool readyToBoost();
    void refillGunDelay();
    void removeRoute();
    void render(int allowHud);
    void resetChargingDrive();
    void resetGunDelay();
    void resetLastHP();
    void resetMovement();
    void revive();
    float right(int frameTime, float delta);
    void roll(int amount);
    void rotate(float rx, float ry, float rz);
    void setActive(bool);
    void setAutoPilot(void* kip);
    void setAutoTurret(bool on);
    void setCollide(bool v);
    void setComputerControlled(bool v);
    void setCurrentSecondaryWeaponIndex(int idx);
    void setDockingCamera();
    void setDockingState(int s);
    void setExhaustVisible(bool param);
    void setFreeLookMode(bool v);
    void setFreeze(bool v);
    void setLevel(void* level);
    void setPosition();
    void setPosition3(float x, float y, float z);
    void setRocketControl(void* gun, void* geo);
    void setRotation(float rx, float ry, float rz);
    void setRoute(int v);
    void setShip(int race, int group);
    void setSpeed(float v);
    void setTargetFollowCamera(void* cam);
    void setThrust(float v);
    void setTurretMode(int enable);
    void setTurretPosition(float x, float y, float z);
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
    void update(int dt, void *radar, void *hud, void *radio, void *script, int arg5, bool arg6, int arg7);
    int updateManeuver();

    // ---- veneer / fragment entry points (forward to the canonical methods) ----
    int getHUD_up();                        // alias of getHUD()
    Vec3 getPosition_up();                  // alias of getPosition()
    void setRoute_init();                   // re-apply the current route (Level init path)
    void rollLeft(int shipField, float amt);   // bank input -> turnHorizontal()
    void rollRight(int shipField, float amt);  // bank input -> turnHorizontal()
    void syncFirstPerson(int v);            // -> hideShipForFirstPersonCameraView()
};
#endif
