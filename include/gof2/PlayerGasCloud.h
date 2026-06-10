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
template <class T> static inline T &F(void *p, uint32_t off) {
    return *(T *)((char *)p + off);
}

struct PlayerGasCloud {
    void* field_0x4;                    // +0x4
    void* field_0x8;                    // +0x8
    uint8_t field_0x25;                 // +0x25
    uint8_t field_0x44;                 // +0x44
    uint8_t field_0x4c;                 // +0x4c
    int field_0x78;                     // +0x78
    int field_0x88;                     // +0x88
    uint8_t field_0xf5;                 // +0xf5
    float field_0x128;                  // +0x128
    float field_0x12c;                  // +0x12c
    float field_0x130;                  // +0x130
    void* field_0x134;                  // +0x134
    void* field_0x138;                  // +0x138
    void* field_0x13c;                  // +0x13c
    void* field_0x140;                  // +0x140
    void* field_0x144;                  // +0x144
    uint8_t field_0x145;                // +0x145 (zeroed as part of a 16-byte vector store)
    void* field_0x148;                  // +0x148
    void* field_0x14c;                  // +0x14c
    void* field_0x150;                  // +0x150
    uint8_t field_0x154;                // +0x154
    int field_0x158;                    // +0x158
    uint8_t field_0x15c;                // +0x15c
    int field_0x160;                    // +0x160
    uint16_t field_0x164;               // +0x164
    int field_0x168;                    // +0x168

    PlayerGasCloud(int param_1, ParticleSystemManager *param_2, AEGeometry *param_3,
                   Vector const &param_4);
    void translate(Vector const &param_1);
    bool isSparkAlive(int param_1);
    void setSparkInSight(int param_1, bool param_2);
    void setPosition(Vector const &param_1);
    void *getSparks();
    Vector getPosition();
    uint8_t hasExploded();
    void render();
};
#endif
