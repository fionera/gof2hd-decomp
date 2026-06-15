#ifndef GOF2_PLAYERFIXEDOBJECT_H
#define GOF2_PLAYERFIXEDOBJECT_H
#include "gof2/common.h"

// Galaxy on Fire 2 - PlayerFixedObject: a stationary/destructible KIPlayer subclass
// (asteroid-station, container, wreck). Owns its render geometry, an optional wrecked
// mesh, two bounding-volume arrays and a one-shot Explosion effect.

class BoundingVolume;

typedef AbyssEngine::AEMath::Vector V3;
typedef AbyssEngine::AEMath::Vector Vector;

class PlayerFixedObject {
public:
    void* player;                             // Player*
    void* geometry;                           // AEGeometry*
    unsigned char empActive;
    int faction;
    Vector position;                          // current world position
    uint8_t field_0x40;
    uint8_t field_0x41;
    unsigned char hasCargo;
    Array<int>* lootList;                     // loot id/amount pairs
    void* level;                              // Level*
    float spawnX;
    float spawnY;
    float spawnZ;
    void* secondaryGeometry;                  // AEGeometry*
    int state;
    uint8_t field_0x8c;
    Vector targetPos;                         // homing-target world position
    int kind;
    int explosionTimer;
    int32_t field_0xf8;
    unsigned char finished;
    void* wreckGeometry;                      // AEGeometry*
    Array<BoundingVolume*>* boundingVolumes;  // active bounding volumes
    Array<BoundingVolume*>* wreckCollision;   // wreck collision volumes
    int32_t deltaTime;
    unsigned char moving;
    Vector respawnPos;                        // spawn position copy used by reset()
    Vector homingTarget;                      // re-homed enemy position snapshot
    Vector homingDir;                         // homingTarget - position (drift direction)
    uint32_t field_0x15c;
    uint64_t field_0x160;
    int32_t targetEnemy;
    int collisionIndex;
    uint8_t field_0x170;
    int32_t field_0x174;
    int32_t intPosX;
    int32_t intPosY;
    int32_t intPosZ;
    uint16_t wreckMeshId;
    void* explosion;                          // Explosion*
    int explosionElapsed;
    int wreckType;
    int rumbleTimer;
    float rumblePercentage;
    int wreckMaterial;
    int dockingType;
    int transportID;
    String name;                              // display name
    uint8_t shipHidden;

    PlayerFixedObject(int kind, int param2, void *player, void *geom,
                      float p5, float p6, float p7, float sx, float sy, float sz);
    ~PlayerFixedObject();

    int collide(float x, float y, float z);
    void ctor(int kind, int param2, void *player, void *geom,
              float p5, float p6, float p7, float sx, float sy, float sz);
    int getDockingType();
    String getName();
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
};
#endif
