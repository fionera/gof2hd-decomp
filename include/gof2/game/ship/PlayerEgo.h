#ifndef GOF2_PLAYEREGO_H
#define GOF2_PLAYEREGO_H
#include "gof2/common.h"
// Vec3 — the 3-component math vector returned by several PlayerEgo methods.
// Alias to the foundation Vector type (AbyssEngine::AEMath::Vector via common.h).
typedef Vector Vec3;
// struct derived from offset-access field map (deterministic field_0xNN naming)
// PlayerEgo — top-level class (NO namespace). Byte-exact decomp scaffold.
// We do NOT model the full layout; fields are accessed by byte offset taken
// from each work-item's target disasm.

struct PlayerEgo;          // opaque; we only ever take a PlayerEgo* and offset-cast.
class RepairBeam;          // element type of the repairBeams array.

// Other game types referenced by pointer only — keep opaque.
// (Player/Ship/etc. forward-declared in fwd.h via common.h; Vector comes from math.h.)

// ---- tiny offset-cast helpers -------------------------------------------------
static inline char*           B(void* p, int off) { return (char*)p + off; }
// I()/P() byte-offset accessors are provided as macros in PlayerEgo.cpp to avoid
// colliding with the identical static-inline definitions in Explosion.h / Hud.h.

class PlayerEgo {
public:
    void* player;                    // +0x0
    AEGeometry* field_0x4;              // +0x4
    AEGeometry* geometry;              // +0x8
    Level* level;                   // +0xc
    LevelScript* levelScript;            // +0x10
    uint8_t field_0x25;                 // +0x25
    void* autoPilotTarget;                  // +0x15c
    Array<RepairBeam*>* repairBeams;        // +0x1b8  active repair-beam effects
    unsigned short cloakMaterial;         // +0x388
    unsigned short cloakMaterial2;         // +0x38c
    unsigned short cloakMaterial3;         // +0x390

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
    // These are the secondary symbols the decompiler split out of the matching
    // full methods; they exist as their own thunks in the binary and are wired up
    // by other translation units, so they get real definitions here.
    int getHUD_up();                        // alias of getHUD()
    Vec3 getPosition_up();                  // alias of getPosition()
    void setRoute_init();                   // re-apply the current route (Level init path)
    void rollLeft(int shipField, float amt);   // bank input -> turnHorizontal()
    void rollRight(int shipField, float amt);  // bank input -> turnHorizontal()
    void syncFirstPerson(int v);            // -> hideShipForFirstPersonCameraView()
};
#endif
