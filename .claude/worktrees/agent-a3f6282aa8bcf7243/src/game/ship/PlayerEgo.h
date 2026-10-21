#ifndef GOF2_PLAYEREGO_H
#define GOF2_PLAYEREGO_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"

// Vec3 — the 3-component math vector returned by several PlayerEgo methods.
// Alias to the foundation Vector type (AbyssEngine::AEMath::Vector via common.h).
typedef Vector Vec3;

// Global engine/game types referenced by pointer only.
class AEGeometry;
class Player;
class Level;
class LevelScript;
class RepairBeam;
class Gun;
class KIPlayer;
class Route;
class TargetFollowCamera;
class Hud;
class Radar;
class Radio;

// PlayerEgo — the player's own ship.
class PlayerEgo {
public:
    void* player;                         // wrapped Player (ship logic / HP / guns)
    AEGeometry* field_0x4;                // transform-local source geometry
    AEGeometry* geometry;                 // hull / ship root geometry
    Level* level;
    LevelScript* levelScript;
    void* field_0x14;                     // KIPlayer-style handle
    int field_0x1c;                       // geometry/camera handle
    uint8_t freeze;                       // frozen / collide-off flag
    uint8_t inWormhole;
    void* turretGeometry;
    void* field_0x2c;                     // geometry node (freed in dtor)
    void* field_0x30;                     // geometry node (freed in dtor)
    void* gunYawGeo;                      // gun yaw (barrel) geometry
    void* gunMuzzleRoot;                  // gun muzzle root geometry
    void* gunExtraGeo;                    // optional extra gun child geometry
    float maneuverParam;                  // scripted-maneuver pitch param
    float field_0x80;
    int targetFollowCamera;               // TargetFollowCamera handle
    void* explosion;
    void* explosion2;
    int field_0xac;                       // emergency-system effect geometry
    int field_0xb0;                       // handling store
    uint8_t switchToStandardCam;          // one-shot "switch to standard cam" flag
    uint8_t field_0xb2;
    int engineSoundId;                    // FMod sound handle for engine/gamma SFX
    int speed;
    float thrust;
    uint8_t freeLookMode;
    int boostSpeed;
    int field_0xcc;                       // boost-percentage period base
    int field_0xd0;                       // boost-rate denominator
    void* boostSoundId;                   // FMod sound handle for boost SFX
    void* rollGeometry;                   // roll/banking geometry node
    float rotX;                           // setPosition source X
    float rotY;                           // setPosition source Y
    float rotZ;                           // setPosition source Z
    float waypointX;                      // steer-to-waypoint target X
    float waypointY;                      // steer-to-waypoint target Y
    void* gunBaseGeo;                     // gun base geometry (also waypoint-Z storage)
    int route;                            // Route*
    int pitchAccumDir;                    // pitch direction flag (-1/+1)
    int yawAccumDir;                      // yaw direction flag (-1/+1)
    int currentSecondaryWeaponIndex;
    float yawAccumulator;
    float pitchAccumulator;
    float yawRate;
    float pitchRate;
    int lastHP;
    int shakeIntensity;
    int boostTimer;
    uint8_t boostingFlag;
    uint8_t collide;                      // collide-disable flag
    uint8_t field_0x145;
    uint8_t field_0x146;                  // boost-enabled flag
    Vector dockOffsetVec;                 // dock-rig offset vector (x/y/z)
    int boostDelay;
    int handling;
    uint8_t autoPilot;
    int autoPilotTarget;                  // current waypoint/landmark
    uint8_t goingToWaypointFlag;
    Vector headingVec;                    // desired heading vector (moveToPosition)
    uint8_t turretMode;                   // turret available/equipped flag
    uint32_t turretCamera;                // turret/dock camera handle
    void* dockCameraNode;                 // dock/turret camera node geometry (root)
    void* dockCameraLeaf;                 // dock/turret camera leaf node
    uint8_t autoTurretEquipped;
    int autoTurretTimer;                  // rescan timer (every 3s)
    int autoTurretFireTimer;
    void* autoTurretTarget;               // current auto-turret target
    void* autoTurretPrevTarget;
    int rocketControlGun;                 // rocket-control gun handle
    int rocketBanking;
    void* dockCameraMid;                  // dock/turret camera mid node
    uint8_t turretActive;                 // in-turret-mode flag
    uint8_t field_0x1a1;
    float lookPitch;                      // turret look pitch accumulator
    float lookYaw;                        // turret look yaw accumulator
    uint8_t cloaked;
    uint8_t chargingCloak;
    void* cloak;                          // cloak device handle
    void* tractorBeam;
    Array<RepairBeam*>* repairBeams;      // active repair-beam effects
    int asteroidTarget;                   // docked/approached asteroid
    int dockingState;                     // docking-procedure flag
    int dockingPointIndex;                // docking-point/landing state index
    int dockApproachDist;
    int dockApproachThreshold;
    int field_0x1d0;
    int dockScaling;
    int miningSettleTimer;
    void* dockStation;                    // station/target being approached
    int miningGame;                       // MiningGame*
    int hackingGame;                      // HackingGame*
    short docked;                         // docked-to-* state word
    uint8_t dockedToStream;
    uint8_t dockingToPlanet;
    int planetDockTimer;
    uint8_t computerControlled;
    int turretPitch;
    int driveCharge;                      // jump-drive charge timer
    int driveChargeMax;
    uint8_t chargingDrive;
    int cloakCharge;
    int cloakRechargeTimer;
    int cloakChargeMax;                   // cloak full-charge threshold
    int cloakDischargeMax;
    int hud;                              // Hud handle
    Vector turretOffsetVec;               // turret-rig offset vector (x/y/z)
    uint8_t collidesWithStationFlag;
    uint8_t hardCoreMode;                 // +0x235 hardcore-mode flag (disables flight assists)
    int hpGaugeImage;                     // HP-gauge image handle
    int dockArrowImage;                   // dock-arrow image handle
    int radarBlipImage1;
    int radarBlipImage2;
    int radarBlipImage3;
    int shakeAccum;                       // hit-shake accumulator timer
    float pitchRamp;                      // pitch ramp rate
    float yawRamp;                        // yaw ramp rate
    double rollAccum;
    double yawAccumD;
    double pitchAccumD;                   // pitch ramp accumulator
    float yawAccumF;                      // yaw ramp accumulator
    int field_0x2a4;
    uint8_t field_0x2a8;
    uint8_t rollDirection;                // roll/auto-level direction (0/1/2)
    Matrix rollMatrix;                    // embedded roll matrix
    void* field_0x2c0;
    uint8_t gunExtraVisible;              // extra-gun visibility flag
    float rotateX;                        // setRotation X
    float rotateY;                        // setRotation Y
    float rotateZ;                        // setRotation Z
    uint8_t rolling;                      // roll-active flag
    uint8_t field_0x2f5;
    int explosionTimer;
    int currentSystem;                    // current SolarSystem handle
    int smokeSystem;                      // smoke particle system
    int explosionSmoke;                   // explosion smoke particle system
    uint8_t field_0x309;                  // visible mirror
    int emergencySystemTimer;
    int emergencyVal1;                    // cached emergency-system value
    Vector emergencyVec;                  // cached emergency transform vector
    float emergencyVal2;
    uint8_t autoLevel;                    // auto-level (horizon-align) flag
    int hitShakeTimer;
    uint8_t hitShake;                     // hit-shake active flag
    uint8_t field_0x32d;
    uint8_t visible;
    uint8_t exhaustVisible;
    uint8_t aboutToReachAutoTargetFlag;
    int maneuverType;                     // scripted-maneuver type
    Vector strafeTargetVec;               // strafe-run target vector (x/y/z)
    Vector facingVec;                     // current facing direction vector
    void* navPoint;                       // nearest navigation point
    uint8_t autoTurretEnabled;
    uint8_t dockedFlag;                   // docked / in-station flag
    void* easeMatrix;                     // EaseInOutMatrix*
    int dockTotalAmount;
    int dockTransferedAmount;
    int cloakRechargeMax;
    int spacePoint;                       // SpacePoint handle (route navigation)
    uint8_t throttleStarted;
    int throttle;
    float strafeAccel;                    // lateral strafe acceleration
    void* strafeNavPoint;                 // nav SpacePoint (also strafe-target storage)
    int cloakMaterial1;                   // cloak material handle
    int cloakMaterial2;
    int cloakMaterial3;
    void* field_0x394;                    // mesh-derived handle
    uint8_t volatileGoods;
    uint8_t lostMiningGameFlag;
    Matrix turretHudMatrix;               // embedded turret/HUD matrix

    PlayerEgo(Player* player);
    ~PlayerEgo() noexcept(false);

    // ---- methods ----
    Vec3 GetDirVector();
    Vec3 GetUpVector();
    void PauseEngineSound();
    void PlayEngineSound();
    void ResumeEngineSound();
    void StopEngineSound();
    unsigned char aboutToReachAutoTarget();
    void addGun(Gun* gun, int x);
    void addGun(Array<Gun*>* arr, int x);
    void addNukeVolatileForce(float v);
    void alignToHorizon();
    void approachAsteroid(Hud *hud, int hud2, Radar *radar);
    int approachDockingPoint(Hud *hud, int /*hud2*/, Radar *radar);
    unsigned char autoTurretIsEnabled();
    void boost();
    unsigned char boosting();
    void calcCollision(Array<KIPlayer*> *candidates);
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
    void moveToPosition(Vector target, bool steer, float speed);
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
    void setAutoPilot(KIPlayer* kip);
    void setAutoTurret(bool on);
    void setCollide(bool v);
    void setComputerControlled(bool v);
    void setCurrentSecondaryWeaponIndex(int idx);
    void setDockingCamera();
    void setDockingState(int s);
    void setExhaustVisible(bool param);
    void setFreeLookMode(bool v);
    void setFreeze(bool v);
    void setLevel(Level* level);
    void setPosition(Vector v);
    void setPosition(float x, float y, float z);   // was setPosition3 (PlayerEgo is not KIPlayer-derived)
    void setRocketControl(Gun* gun, AEGeometry* geo);
    void setRotation(float rx, float ry, float rz);
    void setRoute(Route* v);
    void setShip(int race, int group);
    void setSpeed(float v);
    void setTargetFollowCamera(TargetFollowCamera* cam);
    void setThrust(float v);
    void setTurretMode(bool enable);
    void setTurretPosition(Vector v);
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
};
#endif
