#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include <cstddef>
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "Player.h"
#include "engine/core/AbyssEngine.h"

#include "engine/math/Vector.h"
#include "engine/render/AEGeometry.h"
#include "game/weapons/Gun.h"

class Player;

class AEGeometry;
class Gun;
class Hud;
class Level;
class Route;
class SpacePoint;

class KIPlayer {
public:
    Player *player;
    AEGeometry *geometry;
    AEGeometry *parentGeometry;
    int field_0x10;
    int field_0x14;
    String name;
    uint8_t field_0x24;

    uint8_t field_0x25;

    uint8_t _pad_0x26[2];
    int shipGroup;

    int autoPilotState;

    int field_0x30;
    int field_0x34;
    int field_0x38;

    // Former union { int stealFlag; struct{...}; } at 0x3c: the wide int
    // `stealFlag` is reinterpret_cast<int&>(stealFlagByte) at its use sites.
    char stealFlagByte;
    uint8_t countsAsEnemyExcludeFlag;
    uint8_t field_0x3e;
    char field_0x3f_b;

    // Former union { int field_0x40; struct{...}; } at 0x40: the wide int
    // `field_0x40` is reinterpret_cast<int&>(field_0x40) at its use sites.
    char field_0x40;
    char deadFlag;
    char field_0x42;
    uint8_t reviveLockFlag;

    int field_0x44;
    int field_0x48;
    int hasCargo;
    Array<int> *cargo;
    Level *level;
    float posX;
    float posY;
    float posZ;
    uint32_t field_0x64;
    uint8_t diedWithMissionCrate;

    uint8_t lostMissionCrateToEgo;

    uint8_t field_0x6a;
    uint8_t _pad_0x6b;

    // Former union { Route* route; struct{ uint8_t noTargetFlag, routeByte1..3; }; }
    // at 0x6c: keep the pointer; the status bytes are reinterpret views over it
    // (noTargetFlag == reinterpret_cast<uint8_t&>(route)) at their use sites.
    Route *route;

    uint16_t field_0x70;
    uint8_t field_0x72;
    uint8_t field_0x73;
    uint8_t field_0x74;
    uint8_t field_0x75;
    uint8_t field_0x76;
    uint8_t _pad_0x77;
    AEGeometry *crateGeometry;
    int shipGroupFlag;
    int field_0x80;
    int field_0x84;
    int state;
    int field_0x8c;
    int field_0x90;
    int field_0x94;
    int field_0x98;
    int field_0x9c;
    int field_0xa0;
    int field_0xa4;
    float rotationSpeed;
    int type;
    uint8_t _pad_0xb0;
    uint8_t sleepFlag;
    uint8_t initActiveFlag;
    uint8_t _pad_0xb3;
    Route *initialRoute;
    int field_0xb8;
    int field_0xbc;
    uint8_t field_0xc0;
    uint8_t _pad_0xc1[3];
    Array<uint32_t> *spacePointIds;
    int field_0xc8;
    Array<SpacePoint *> *spacePoints;
    int carriesMissionCrate;
    uint32_t jumpSphere;
    int field_0xd8;
    uint8_t wingmanFlag;
    uint8_t _pad_0xdd[3];
    int wingmanCommand;
    int field_0xe4;
    KIPlayer *wingmanTarget;
    uint8_t jumperFlag;
    uint8_t _pad_0xed[7];
    uint8_t jumpDone;
    uint8_t visibleFlag;
    uint8_t _pad_0xf6[2];
    int engineSoundEvent;
    uint32_t field_0xfc;
    uint8_t field_0x100;
    uint8_t field_0x101;
    uint8_t _pad_0x102[2];
    int field_0x104;
    int field_0x108;
    int field_0x10c;
    int field_0x110;
    int field_0x114;
    int field_0x118;
    int field_0x11c;
    int field_0x120;

    KIPlayer(int faction, int group, Player *player, AEGeometry *geom,
             float x, float y, float z, bool active);

    virtual ~KIPlayer();

    void PauseEngineSound();

    void PlayEngineSound();

    void ResumeEngineSound();

    void StopEngineSound();

    virtual void awake();

    void captureCrate(Hud *hud);

    int cargoAvailable();

    void createCrate(int type);

    SpacePoint *getNearestDockingPoint(const Vector &dir);

    SpacePoint *getNearestNavigationPoint(const Vector &dir, SpacePoint *target);

    virtual Vector getPosition();

    int getSpeed();

    int getType();

    bool isDead();

    bool isDocked();

    bool isDying();

    uint8_t isEnemy();

    uint8_t isJumper();

    uint8_t isVisible();

    uint8_t isWingMan();

    void jump();

    virtual int collide(float x, float y, float z);

    virtual int outerCollide(const Vector &v);

    virtual int outerCollide(float x, float y, float z);

    virtual void render();

    virtual void update(int dt);

    void reset();

    void setActive(bool active);

    void setDead();

    void setEnemies(Array<Player *> *enemies);

    void setInitActive(bool active);

    void setJumpSphere(uint32_t sphere);

    void setJumper(bool b);

    virtual void setPosition(float x, float y, float z);

    void setPosition(const Vector &v);

    void setRotationSpeed(float speed);

    void setRoute(Route *route);

    virtual void setShipGroup(AEGeometry *geom, int group, bool flag);

    void setSpacePoints(Array<SpacePoint *> *pts);

    void setState(int state);

    void setToSleep();

    void setVisible(bool visible);

    void setWingman(bool b, int cmd);

    virtual void setWingmanCommand(int cmd, KIPlayer *target);

    virtual void setSpeed(float v);

    virtual void revive();

    virtual void push(int dt);

    virtual void translate(const Vector &v);

    virtual void initPush(const Vector &target, int radius);

    Route *getRoute();

    Array<SpacePoint *> *getSpacePoints();

    virtual void setLevel(Level *lvl);

    void addGun(Gun *gun, int slot);

    void addGun(Array<Gun *> *guns, int slot);

    int levelCollision(Vector *out, long long flags);

    void enableExplosion();

    void setInitialRotation(Vector rotation);

    virtual Vector getProjectionVector(const Vector &v);

    Vector getCollisionNormal(const Vector &position);

    virtual Vector projectCollisionOnSurface(const Vector &position);
};

#if __SIZEOF_POINTER__ == 4

static_assert(sizeof(KIPlayer) == 292, "KIPlayer must be 292 bytes (binary layout)");
// Former-union sites (offsets must be byte-identical to the eliminated unions):
static_assert(offsetof(KIPlayer, stealFlagByte) == 0x3c, "KIPlayer::stealFlagByte must be at 0x3c");
static_assert(offsetof(KIPlayer, field_0x40) == 0x40, "KIPlayer::field_0x40 must be at 0x40");
static_assert(offsetof(KIPlayer, deadFlag) == 0x41, "KIPlayer::deadFlag must be at 0x41");
static_assert(offsetof(KIPlayer, field_0x44) == 0x44, "KIPlayer::field_0x44 must be at 0x44");
static_assert(offsetof(KIPlayer, posX) == 0x58, "KIPlayer::posX must be at 0x58");
static_assert(offsetof(KIPlayer, route) == 0x6c, "KIPlayer::route must be at 0x6c");
static_assert(offsetof(KIPlayer, field_0x70) == 0x70, "KIPlayer::field_0x70 must be at 0x70");
static_assert(offsetof(KIPlayer, field_0x74) == 0x74, "KIPlayer::field_0x74 must be at 0x74");
static_assert(offsetof(KIPlayer, field_0x76) == 0x76, "KIPlayer::field_0x76 must be at 0x76");
#endif

#endif
