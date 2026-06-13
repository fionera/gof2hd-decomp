#ifndef GOF2_PLAYERFIXEDOBJECT_H
#define GOF2_PLAYERFIXEDOBJECT_H
#include "gof2/common.h"
#include "gof2/game/world/Station.h"   // provides the single shared `struct RetStr`
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 - PlayerFixedObject class. Android libgof2hdaa.so, armv7 Thumb.
// Top-level class (no AbyssEngine namespace). Field offsets recovered per-method from the
// target disassembly; we do NOT model a full layout - access fields via byte-offset casts.


// Opaque referenced types.
struct PlayerFixedObject;
struct BoundingVolume;
struct AEGeometry;
struct Player;

// `struct RetStr` (3-word by-value String ABI aggregate) is provided by gof2/Station.h.
// V3: 3-float vector return type (AbyssEngine::AEMath::Vector ABI).
typedef AbyssEngine::AEMath::Vector V3;
typedef AbyssEngine::AEMath::Vector Vector;

// Field accessors via byte offset.

class PlayerFixedObject {
public:
    void* player;                    // +0x4
    void* geometry;                    // +0x8
    unsigned char empActive;           // +0x24
    int faction;                     // +0x28
    float posX;                   // +0x2c
    float posY;                   // +0x30
    float posZ;                   // +0x34
    uint8_t field_0x40;                 // +0x40
    uint8_t field_0x41;                 // +0x41
    unsigned char hasCargo;           // +0x4c
    void* lootList;                   // +0x50
    void* level;                   // +0x54
    float spawnX;                   // +0x58
    float spawnY;                   // +0x5c
    float spawnZ;                   // +0x60
    void* secondaryGeometry;                   // +0x78
    int state;                     // +0x88
    uint8_t field_0x8c;                 // +0x8c
    float targetX;                   // +0x90
    float targetY;                   // +0x94
    float targetZ;                   // +0x98
    int kind;                     // +0xac
    int explosionTimer;                     // +0xd8
    int32_t field_0xf8;                 // +0xf8
    unsigned char finished;          // +0x101
    void* wreckGeometry;                  // +0x124
    void* boundingVolumes;                  // +0x128
    void* wreckCollision;                  // +0x12c
    int32_t deltaTime;                // +0x130
    unsigned char moving;          // +0x134
    uint64_t field_0x138;               // +0x138
    uint64_t field_0x140;               // +0x140
    unsigned int field_0x144;           // +0x144
    uint64_t field_0x148;               // +0x148
    unsigned int field_0x14c;           // +0x14c
    uint64_t field_0x150;               // +0x150
    float field_0x154;                  // +0x154
    uint64_t field_0x158;               // +0x158
    uint64_t field_0x160;               // +0x160
    int32_t targetEnemy;                // +0x168
    int collisionIndex;                    // +0x16c
    uint8_t field_0x170;                // +0x170
    int32_t field_0x174;                // +0x174
    int32_t intPosX;                // +0x178
    int32_t intPosY;                // +0x17c
    int32_t intPosZ;                // +0x180
    uint16_t wreckMeshId;               // +0x184
    void* explosion;                  // +0x18c
    int explosionElapsed;                    // +0x190
    int wreckType;                    // +0x194
    int rumbleTimer;                    // +0x198
    float rumblePercentage;                  // +0x19c
    int wreckMaterial;                    // +0x1a0
    int dockingType;                    // +0x1a4
    int transportID;                    // +0x1a8
    uint8_t shipHidden;                // +0x1b8

    // ---- methods (converted from free functions) ----
    PlayerFixedObject(int kind, int param2, void *player, void *geom, float p5, float p6, float p7, float sx, float sy, float sz);
    ~PlayerFixedObject();
    int collide(float x, float y, float z);
    void ctor(int kind, int param2, void *player, void *geom, float p5, float p6, float p7, float sx, float sy, float sz);
    int getDockingType();
    RetStr getName();
    V3 getPosition();
    V3 getProjectionVector();
    int getTransportID();
    void hideShip();
    void moveForward(int amount);
    int outerCollide(float x, float y, float z);
    void outerCollide_vec(Vector v);
    V3 projectCollisionOnSurface(void *vec);
    void render();
    void reset();
    void setBV(BoundingVolume *bv);
    void setBV_arr(Array<BoundingVolume *> *bv);
    void setDeadButSelectable();
    void setDockingType(int v);
    void setExhaustVisible(bool v);
    void setMoving(bool v);
    void * setName(String *name);
    void setPosition3(float x, float y, float z);
    void setPosition_vec(const Vector &v);
    void setTransportID(int v);
    void setWreckedMeshId(int meshId);
    void translate(const Vector &d);
    void update(int dt);

    // Destructor tail: after the PlayerFixedObject-owned members are released, the base
    // (Fighter/Player) subobject is torn down here. Returns `this` (the deleting variant
    // forwards the result to operator delete).
    void *baseDtor();
};
#endif
