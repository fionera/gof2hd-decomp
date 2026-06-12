#ifndef GOF2_KIPLAYER_H
#define GOF2_KIPLAYER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - KIPlayer class (enemy AI). Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.


// Opaque referenced types.
struct KIPlayer;
struct Route;
struct Gun;
struct SpacePoint;
struct Level;



// AbyssEngine::AEMath::Vector - 3 floats. Passed by const ref / value.
namespace AbyssEngine { namespace AEMath {

} 
    // ---- methods (converted from free functions) ----
    void PauseEngineSound();
    void PlayEngineSound();
    void ResumeEngineSound();
    void StopEngineSound();
    void addGun_a();
    void addGun_b();
    void awake();
    void captureCrate(void *hud);
    int cargoAvailable();
    void createCrate(int type);
    void ctor(int faction, int group, void *player, void *geom, float x, float y, float z, bool active);
    void * getNearestDockingPoint(Vector *dir);
    void * getNearestNavigationPoint(Vector *dir, void *target);
    Vector getPosition();
    int getType();
    bool isDead();
    bool isDocked();
    bool isDying();
    uint8_t isEnemy();
    uint8_t isJumper();
    uint8_t isVisible();
    uint8_t isWingMan();
    void jump();
    void outerCollide(const Vector &v);
    void render();
    void reset();
    void setActive();
    void setDead();
    void setEnemies();
    void setInitActive();
    void setJumpSphere(uint32_t sphere);
    void setJumper(bool b);
    void setPosition3(float x, float y, float z);
    void setPosition_vec(const Vector &v);
    void setRotationSpeed(float speed);
    void setRoute(Route *route);
    void setShipGroup(int param2, int flag, int cond);
    void setSpacePoints(Array<SpacePoint *> *pts);
    void setState(int state);
    void setToSleep();
    void setVisible(bool visible);
    void setWingman(bool b, int cmd);
    void setWingmanCommand(int cmd, KIPlayer *target);
    void translate(const Vector &v);
}
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.

class KIPlayer {
public:
    void* player;                    // +0x4   Player*
    void* geometry;                    // +0x8
    void* parentGeometry;                    // +0xc
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int field_0x24;                     // +0x24
    uint8_t field_0x25;                 // +0x25
    int shipGroup;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    char field_0x3f;                    // +0x3f
    int stealFlag;                     // +0x3c
    int field_0x40;                     // +0x40
    char field_0x42;                    // +0x42
    int field_0x44;                     // +0x44
    int field_0x48;                     // +0x48
    int field_0x4c;                     // +0x4c
    void* cargo;                   // +0x50
    void* level;                   // +0x54  Item/cargo holder
    float posX;                   // +0x58
    float posY;                   // +0x5c
    float posZ;                   // +0x60
    unsigned int field_0x64;            // +0x64
    uint8_t field_0x68;                 // +0x68
    uint8_t field_0x69;                 // +0x69
    uint8_t field_0x6a;                 // +0x6a
    Route* route;                  // +0x6c
    int field_0x70;                     // +0x70
    uint8_t field_0x72;                 // +0x72
    uint16_t field_0x73;                // +0x73
    uint8_t field_0x75;                 // +0x75
    uint8_t field_0x76;                 // +0x76
    void* crateGeometry;                   // +0x78
    int shipGroupFlag;                     // +0x7c
    int field_0x80;                     // +0x80
    int field_0x84;                     // +0x84
    int state;                     // +0x88
    int field_0x8c;                     // +0x8c
    int field_0x90;                     // +0x90
    int field_0x94;                     // +0x94
    int field_0x98;                     // +0x98
    int field_0x9c;                     // +0x9c
    int field_0xa0;                     // +0xa0
    int field_0xa4;                     // +0xa4
    float rotationSpeed;                   // +0xa8
    int type;                     // +0xac
    uint8_t sleepFlag;                 // +0xb1
    uint8_t initActiveFlag;                 // +0xb2
    Route* initialRoute;                  // +0xb4
    int field_0xbc;                     // +0xbc
    uint8_t field_0xc0;                 // +0xc0
    void* field_0xc4;                   // +0xc4
    int field_0xc8;                     // +0xc8
    void* spacePoints;                   // +0xcc
    int field_0xd0;                     // +0xd0
    uint32_t jumpSphere;                // +0xd4
    int field_0xd8;                     // +0xd8
    uint8_t wingmanFlag;                 // +0xdc
    int wingmanCommand;                     // +0xe0
    int field_0xe4;                     // +0xe4
    KIPlayer* wingmanTarget;               // +0xe8
    uint8_t jumperFlag;                 // +0xec
    uint8_t jumpDone;                 // +0xf4
    uint8_t visibleFlag;                 // +0xf5
    int engineSoundEvent;                     // +0xf8
    uint32_t field_0xfc;                // +0xfc
    uint8_t field_0x100;                // +0x100
    uint8_t field_0x101;                // +0x101
    int field_0x104;                    // +0x104
    int field_0x10c;                    // +0x10c
    int field_0x110;                    // +0x110
    int field_0x114;                    // +0x114
    int field_0x118;                    // +0x118
    int field_0x11c;                    // +0x11c
    int field_0x120;                    // +0x120

    // ---- methods (converted from free functions) ----
    void PauseEngineSound();
    void PlayEngineSound();
    void ResumeEngineSound();
    void StopEngineSound();
    void addGun_a();
    void addGun_b();
    void awake();
    void captureCrate(void *hud);
    int cargoAvailable();
    void createCrate(int type);
    void ctor(int faction, int group, void *player, void *geom, float x, float y, float z, bool active);
    void * getNearestDockingPoint(Vector *dir);
    void * getNearestNavigationPoint(Vector *dir, void *target);
    Vector getPosition();
    int getType();
    bool isDead();
    bool isDocked();
    bool isDying();
    uint8_t isEnemy();
    uint8_t isJumper();
    uint8_t isVisible();
    uint8_t isWingMan();
    void jump();
    void outerCollide(const Vector &v);
    void render();
    void reset();
    void setActive();
    void setDead();
    void setEnemies();
    void setInitActive();
    void setJumpSphere(uint32_t sphere);
    void setJumper(bool b);
    void setPosition3(float x, float y, float z);
    void setPosition_vec(const Vector &v);
    void setRotationSpeed(float speed);
    void setRoute(Route *route);
    void setShipGroup(int param2, int flag, int cond);
    void setSpacePoints(Array<SpacePoint *> *pts);
    void setState(int state);
    void setToSleep();
    void setVisible(bool visible);
    void setWingman(bool b, int cmd);
    void setWingmanCommand(int cmd, KIPlayer *target);
    void translate(const Vector &v);
    Route *getRoute();
    Array<SpacePoint *> *getSpacePoints();
    void setLevel(Level *lvl);
};
#endif
