#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include "engine/core/Array.h"
#include "../../engine/core/AEString.h"
#include "fieldaccess.h"
#include "Player.h"
#include "engine/core/AbyssEngine.h"

#include "engine/math/Vector.h"
#include "engine/render/AEGeometry.h"
#include "game/ui/Hud.h"
#include "game/weapons/Gun.h"
#include "game/world/Level.h"
#include "game/world/Route.h"
#include "game/world/SpacePoint.h"

class KIPlayer {
public:


    Player *player;
    AEGeometry *geometry;
    AEGeometry *parentGeometry;
    int field_0x10;
    int field_0x14;
    String name;
    int field_0x24;

    union {
        uint8_t garbledWingmanFlag;
        uint8_t field_0x25;
    };

    int shipGroup;

    union {
        int autoPilotState;
        int field_0x2c;
    };

    int field_0x30;
    int field_0x34;

    union {
        int stealFlag;

        struct {
            char stealFlagByte;
            uint8_t countsAsEnemyExcludeFlag;
            uint8_t field_0x3e;
            char field_0x3f_b;
        };
    };

    union {
        char field_0x3f;

        struct {
            char field_0x40b;
            char deadFlag;
            char field_0x42b;
            uint8_t reviveLockFlag;
        };
    };

    int field_0x40;
    char field_0x42;
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

    union {
        uint8_t lostMissionCrateToEgo;
        uint8_t inactiveFlag;
        uint8_t proximityAlarmFlag;
    };

    uint8_t field_0x6a;

    union {
        Route *route;

        struct {
            uint8_t noTargetFlag;
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
    int carriesMissionCrate;
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

    void addGun(Gun *gun, int slot) {
    }

    void addGun(Array<Gun *> *guns, int slot);

    int levelCollision(Vector *out, long long flags);

    void enableExplosion();

    void setInitialRotation(Vector rotation);

    virtual Vector getProjectionVector(const Vector &v);

    Vector getCollisionNormal(const Vector &position);

    virtual Vector projectCollisionOnSurface(const Vector &position);
};
#endif
