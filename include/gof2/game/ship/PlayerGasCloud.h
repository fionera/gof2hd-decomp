#ifndef GOF2_PLAYERGASCLOUD_H
#define GOF2_PLAYERGASCLOUD_H
#include "gof2/common.h"

// Galaxy on Fire 2 -- PlayerGasCloud: a destructible gas/cargo cloud. When hit it
// "explodes" into a swarm of spark shards (each a small AEGeometry) that drift, fade
// and can be collected by the player's turret. Top-level, no namespace; the math
// argument types (Vector) live in AbyssEngine::AEMath.

class Player;
class Level;
class AEGeometry;
class ParticleSystemManager;

class PlayerGasCloud {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    void*    vtable;
    Player*  player;
    AEGeometry* geometry;
    uint8_t  field_0x25;
    uint8_t  field_0x44;
    uint8_t  field_0x4c;
    Level*   level;                       // owning level
    int      field_0x78;
    int      state;
    uint8_t  active;
    Vector   center;                      // cloud centre
    AEGeometry* modelGeometry;            // intact-cloud render geometry
    Array<AEGeometry*>* sparkGeometries;  // per-spark render geometry
    Array<Vector*>*     sparkVelocities;  // per-spark velocity (owned Vector*)
    Array<float>*       sparkLife;
    Array<float>*       sparkLifeMin;
    Array<float>*       sparkScale;
    Array<int>*         sparkTimers;
    Array<bool>*        sparkInSight;
    uint8_t  exploded;
    int      elapsedSinceExplosion;
    uint8_t  settled;
    int      itemId;
    uint16_t sparkMeshId;
    int      cloudMeshId;

    PlayerGasCloud(int itemId, ParticleSystemManager* particles, AEGeometry* geometry,
                   const Vector& position);
    ~PlayerGasCloud();

    void translate(const Vector& v);
    bool isSparkAlive(int index);
    void setSparkInSight(int index, bool inSight);
    void setPosition(const Vector& position);
    void* getSparks();
    Vector getPosition();
    uint8_t hasExploded();
    void render();
    void explode(int itemIndex, Vector src, float radius);
    void update(int dt);
};
#endif
