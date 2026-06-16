#ifndef GOF2_PLAYERFIXEDOBJECT_H
#define GOF2_PLAYERFIXEDOBJECT_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ship/KIPlayer.h"

// Galaxy on Fire 2 - PlayerFixedObject: a stationary/destructible KIPlayer subclass
// (asteroid-station, container, wreck). Owns its render geometry, an optional wrecked
// mesh, two bounding-volume arrays and a one-shot Explosion effect. The engine drives it
// through the shared actor vtable (render/setPosition3/setLevel/...), so it derives from
// KIPlayer (its ctor constructs a KIPlayer base subobject); player/geometry/level/state
// are inherited.

class BoundingVolume;
class AEGeometry;
class Explosion;

typedef AbyssEngine::AEMath::Vector V3;

class PlayerFixedObject : public KIPlayer {
public:
    unsigned char empActive;
    int faction;
    Vector position;                          // current world position
    uint8_t field_0x40;
    uint8_t field_0x41;
    unsigned char hasCargo;
    Array<int>* lootList;                     // loot id/amount pairs
    float spawnX;
    float spawnY;
    float spawnZ;
    AEGeometry* secondaryGeometry;            // optional cargo/secondary mesh
    uint8_t collisionEnabled;                 // gate for collide()/outerCollide()
    Vector targetPos;                         // homing-target world position
    int kind;
    int explosionTimer;
    int32_t aiActiveCounter;                  // KIPlayer "active for tutorial" countdown
    unsigned char finished;
    AEGeometry* wreckGeometry;                // wrecked-ship mesh
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
    Explosion* explosion;                     // one-shot death explosion effect
    int explosionElapsed;
    int wreckType;
    int rumbleTimer;
    float rumblePercentage;
    int wreckMaterial;
    int dockingType;
    int transportID;
    String name;                              // display name
    uint8_t shipHidden;

    // The three floats are the spawn position; they seed both the KIPlayer base
    // position and this object's position (the binary passes them once).
    PlayerFixedObject(int kind, int param2, void *player, void *geom,
                      float x, float y, float z);
    ~PlayerFixedObject();

    int collide(float x, float y, float z) override;   // actor vtable slot +0x38
    void ctor(int kind, int param2, void *player, void *geom, float x, float y, float z);
    int getDockingType();
    String getName();
    V3 getPosition();
    V3 getProjectionVector();
    int getTransportID();
    void hideShip();
    void moveForward(int amount);
    int outerCollide(float x, float y, float z) override;   // actor vtable slot +0x3c
    void outerCollide_vec(Vector v);
    V3 projectCollisionOnSurface(void *vec);
    void render() override;
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
    void update(int dt) override;
};
#endif
