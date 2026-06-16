#ifndef GOF2_PLAYERGASCLOUD_H
#define GOF2_PLAYERGASCLOUD_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "game/ship/KIPlayer.h"

// Galaxy on Fire 2 -- PlayerGasCloud: a destructible gas/cargo cloud. When hit it
// "explodes" into a swarm of spark shards (each a small AEGeometry) that drift, fade
// and can be collected by the player's turret. A KIPlayer subclass (the engine drives
// it through the shared actor vtable: render/setPosition3/setLevel/...); the math
// argument types (Vector) live in AbyssEngine::AEMath.

class AEGeometry;
class ParticleSystemManager;

class PlayerGasCloud : public KIPlayer {
public:
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
