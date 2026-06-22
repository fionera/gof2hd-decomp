#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/Vector.h"

class Player;
class Route;
class Gun;
class SpacePoint;
class Level;
class Hud;
class AEGeometry;

class KIPlayer {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    Player *player;
    AEGeometry *geometry;
    AEGeometry *parentGeometry;
    int field_0x10;
    int field_0x14;
    String name;
    int field_0x24;
    // +0x25: ship "active/spawned" flag (1 byte). Level::assignGuns() tests it through a
    // garbled decompile that read it off the array-slot address (&(*enemies)[i] + 0x25);
    // the real read is this byte on the dereferenced KIPlayer element. Aliased so the
    // garbledWingmanFlag access and existing field_0x25 reads resolve to the same byte.
    union {
        uint8_t garbledWingmanFlag;
        uint8_t field_0x25;
    };

    int shipGroup;
    // +0x2c: autopilot state slot. PlayerEgo::setAutoPilot() zeroes it through the
    // active KIPlayer handle (field_0x14) when autopilot is cleared. 4-byte int.
    union {
        int autoPilotState;
        int field_0x2c;
    };

    int field_0x30;
    int field_0x34;
    // stealFlag (int) overlays two named flag bytes used by the enemy-count scan in
    // Level.cpp / steal-cargo logic: byte 0 (+0x3c) is the steal-flag byte tested when
    // tallying live enemies; byte 1 (+0x3d) is the "counts-as-enemy" exclude flag
    // (XOR'd into the running enemy count). Kept as a union so existing `stealFlag`
    // int access and the per-byte member access resolve to the same memory.
    union {
        int stealFlag;

        struct {
            char stealFlagByte; // +0x3c
            uint8_t countsAsEnemyExcludeFlag; // +0x3d
            uint8_t field_0x3e;
            char field_0x3f_b;
        };
    };

    // The original 0x40 int is a packed group of flag bytes: +0x41 is the "dead" flag
    // and +0x43 is the revive-lock flag, both tested via raw byte reads in Level.cpp.
    // Modeled as named bytes (was: char field_0x3f; int field_0x40) without shifting
    // any following member; field_0x40 int alias kept for TUs that read it whole.
    union {
        char field_0x3f; // +0x40 (the original single byte at this slot)
        struct {
            char field_0x40b; // +0x40
            char deadFlag; // +0x41
            char field_0x42b; // +0x42
            uint8_t reviveLockFlag; // +0x43
        };
    };

    int field_0x40; // +0x44 (unchanged: int at 0x44)
    char field_0x42; // +0x48
    int field_0x44; // +0x4c
    int field_0x48;
    int hasCargo; // +0x4c
    Array<int> *cargo;
    Level *level;
    float posX;
    float posY;
    float posZ;
    uint32_t field_0x64;
    uint8_t diedWithMissionCrate; // +0x68
    // +0x71 in the model's byte layout: this slot doubles as the "inactive"/"proximity
    // alarm" flag the enemy-count scan and PlayerEgo proximity check test by raw byte
    // read. Aliased here so those reads become clean member access (same byte).
    union {
        uint8_t lostMissionCrateToEgo; // +0x69
        uint8_t inactiveFlag;
        uint8_t proximityAlarmFlag;
    };

    uint8_t field_0x6a;
    // route is a pointer, but the enemy/turret scans raw-read its first byte (+0x74) as a
    // flag: "no target" in PlayerEgo target selection and "turret spawned" set by
    // Level::createFighterTurrets(). Overlaid via union so route stays a Route* and the
    // flag byte resolves to the same +0x74 memory without shifting any member.
    union {
        Route *route;

        struct {
            uint8_t noTargetFlag; // +0x74 (== turretSpawnedFlag)
            uint8_t routeByte1;
            uint8_t routeByte2;
            uint8_t routeByte3;
        };
    };

    int field_0x70;
    uint8_t field_0x72;
    uint16_t field_0x73;
    uint8_t field_0x75;
    uint8_t field_0x76;
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
    uint8_t sleepFlag;
    uint8_t initActiveFlag;
    Route *initialRoute;
    int field_0xbc;
    uint8_t field_0xc0;
    Array<uint32_t> *spacePointIds;
    int field_0xc8;
    Array<SpacePoint *> *spacePoints;
    int carriesMissionCrate; // +0xd0
    uint32_t jumpSphere;
    int field_0xd8;
    uint8_t wingmanFlag;
    int wingmanCommand;
    int field_0xe4;
    KIPlayer *wingmanTarget;
    uint8_t jumperFlag;
    uint8_t jumpDone;
    uint8_t visibleFlag;
    int engineSoundEvent;
    uint32_t field_0xfc;
    uint8_t field_0x100;
    uint8_t field_0x101;
    int field_0x104;
    int field_0x10c;
    int field_0x110;
    int field_0x114;
    int field_0x118;
    int field_0x11c;
    int field_0x120;

    KIPlayer(int faction, int group, Player *player, AEGeometry *geom,
             float x, float y, float z, bool active);

    // Virtual: the actor rosters hold KIPlayer* pointing at the ship subclasses, so
    // deleting through the base must run the subclass destructor.
    virtual ~KIPlayer();

    void PauseEngineSound();

    void PlayEngineSound();

    void ResumeEngineSound();

    void StopEngineSound();

    // actor vtable slot +0xc: base flips state and wakes the wrapped Player; subclasses override.
    virtual void awake();

    void captureCrate(Hud *hud);

    int cargoAvailable();

    void createCrate(int type);

    SpacePoint *getNearestDockingPoint(const Vector &dir);

    SpacePoint *getNearestNavigationPoint(const Vector &dir, SpacePoint *target);

    virtual Vector getPosition(); // actor vtable slot +0x28 (Ghidra-verified)
    // A bare actor has no propulsion of its own, so it reports zero speed.
    // Ship subclasses (PlayerEgo, ...) report their real velocity.
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

    // actor vtable slot +0x38 (base stub returns 0; subclasses override)
    virtual int outerCollide(const Vector &v); // actor vtable slot +0x40 (unpacks, forwards to float form)
    virtual int outerCollide(float x, float y, float z);

    // actor vtable slot +0x3c (base stub returns 0; subclasses override)
    // Polymorphic: PlayerJunk/PlayerAsteroid/PlayerCreature/PlayerTurret override this.
    // Level::render()/renderPause() dispatch through it for every actor in the rosters.
    virtual void render();

    // actor vtable slot +0x34; Level::update() dispatches it for every actor in the rosters.
    // Base actor does nothing per tick (binary stub is a pass-through); subclasses override.
    virtual void update(int dt);

    void reset();

    void setActive(bool active);

    void setDead();

    void setEnemies(Array<Player *> *enemies);

    void setInitActive(bool active);

    void setJumpSphere(uint32_t sphere);

    void setJumper(bool b);

    // Polymorphic position setter dispatched through the actor vtable from Level's
    // spawn/respawn paths (createMission/updateOrbit/createSentryGuns).
    virtual void setPosition(float x, float y, float z);

    // Vector-form position setter: stores the world position into the render
    // geometry, then mirrors the resulting transform onto the wrapped Player
    // (composing with the parent ship-group transform when present).
    void setPosition(const Vector &v);

    void setRotationSpeed(float speed);

    void setRoute(Route *route);

    // actor vtable slot +0x8: (re)parents the render geometry to the given ship-group node.
    virtual void setShipGroup(AEGeometry *geom, int group, bool flag);

    void setSpacePoints(Array<SpacePoint *> *pts);

    void setState(int state);

    void setToSleep();

    void setVisible(bool visible);

    void setWingman(bool b, int cmd);

    virtual void setWingmanCommand(int cmd, KIPlayer *target); // actor vtable slot +0x10
    virtual void setSpeed(float v); // actor vtable slot +0x1c
    virtual void revive(); // actor vtable slot +0x18 (base no-op)
    virtual void push(int dt); // actor vtable slot +0x30 (base no-op)
    // actor vtable slot +0x20: shifts the geometry (and route) by a world-space delta.
    virtual void translate(const Vector &v);

    // actor vtable slot +0x2c: arm a bomb/explosion push impulse toward the given point.
    virtual void initPush(const Vector &target, int radius);

    Route *getRoute();

    Array<SpacePoint *> *getSpacePoints();

    // Polymorphic: PlayerFighter/PlayerEgo override this to also wire up their engine-trail
    // particle systems. Dispatched through the actor vtable right after construction.
    virtual void setLevel(Level *lvl);

    // Installs a gun (or a whole gun list) into the given weapon slot of the wrapped Player.
    void addGun(Gun *gun, int slot);

    void addGun(Array<Gun *> *guns, int slot);

    // Bounding-volume collision query against the level geometry. A bare KIPlayer has no
    // collision surface, so it reports no hit.
    int levelCollision(Vector *out, long long flags);

    // Arms the death explosion. The base actor has no explosion model; subclasses override.
    void enableExplosion();

    // Records the spawn orientation. The base actor keeps no rotation state.
    void setInitialRotation(Vector rotation);

    // Collision-query virtuals inherited from the bounding-volume interface. A KIPlayer
    // does not expose a surface to project onto, so all three return a zeroed vector.
    // getProjectionVector @ actor vtable slot +0x50; projectCollisionOnSurface @ +0x58.
    virtual Vector getProjectionVector(const Vector &v);

    Vector getCollisionNormal(const Vector &position);

    virtual Vector projectCollisionOnSurface(const Vector &position);
};
#endif
