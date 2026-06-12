#ifndef GOF2_PLAYERGASCLOUD_H
#define GOF2_PLAYERGASCLOUD_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PlayerGasCloud (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "PlayerGasCloud::..." (the class is NOT in a
// namespace; only argument types like AbyssEngine::AEMath::Vector are).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.


struct PlayerGasCloud;
struct ParticleSystemManager;
struct AEGeometry;

using Vector = AbyssEngine::AEMath::Vector;

// Templated byte-offset field accessor over an opaque container pointer (the spark Array<T>
// objects share the {count@+0,data@+4,capacity@+8} layout). F<int>(arr, 0x0) reads the count,
// F<int*>(arr, 0x4) reads the data pointer, etc.

class PlayerGasCloud {
public:
    void* player;                    // +0x4
    void* geometry;                    // +0x8
    uint8_t field_0x25;                 // +0x25
    uint8_t field_0x44;                 // +0x44
    uint8_t field_0x4c;                 // +0x4c
    int field_0x78;                     // +0x78
    int state;                     // +0x88
    uint8_t active;                 // +0xf5
    float centerX;                  // +0x128
    float centerY;                  // +0x12c
    float centerZ;                  // +0x130
    void* modelGeometry;                  // +0x134
    void* sparkGeometries;                  // +0x138
    void* sparkVelocities;                  // +0x13c
    void* sparkLife;                  // +0x140
    void* sparkLifeMin;                  // +0x144
    uint8_t field_0x145;                // +0x145 (zeroed as part of a 16-byte vector store)
    void* sparkScale;                  // +0x148
    void* sparkTimers;                  // +0x14c
    void* sparkInSight;                  // +0x150
    uint8_t exploded;                // +0x154
    int elapsedSinceExplosion;                    // +0x158
    uint8_t settled;                // +0x15c
    int itemId;                    // +0x160
    uint16_t sparkMeshId;               // +0x164
    int cloudMeshId;                    // +0x168

    PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                   Vector const &param_4);
    ~PlayerGasCloud();
    void translate(Vector const &param_1);
    bool isSparkAlive(int param_1);
    void setSparkInSight(int param_1, bool param_2);
    void setPosition(Vector const &param_1);
    void *getSparks();
    Vector getPosition();
    uint8_t hasExploded();
    void render();
    void *baseDtor();   // ~PlayerGasCloud tail: SpacePoint base-subobject teardown
};
#endif
