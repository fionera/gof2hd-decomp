#ifndef GOF2_PLAYERASTEROID_H
#define GOF2_PLAYERASTEROID_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {



namespace AEMath {



} // namespace AEMath

} // namespace AbyssEngine

using String = AbyssEngine::String;
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct AEGeometry;
struct Player;
struct Explosion;
struct TargetFollowCamera;
struct ArrayInt;







extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr);

struct PlayerAsteroid {
    void* field_0x0;                    // +0x0
    Player* field_0x4;                  // +0x4
    AEGeometry* field_0x8;              // +0x8
    uint8_t field_0x3c;                 // +0x3c
    uint8_t field_0x4c;                 // +0x4c
    ArrayInt* field_0x50;               // +0x50
    AEGeometry* field_0x78;             // +0x78
    int field_0x88;                     // +0x88
    uint8_t field_0xf5;                 // +0xf5
    int field_0x104;                    // +0x104
    int field_0x108;                    // +0x108
    float field_0x118;                  // +0x118
    float field_0x11c;                  // +0x11c
    float field_0x120;                  // +0x120
    uint8_t field_0x124;                // +0x124
    int field_0x128;                    // +0x128
    Explosion* field_0x12c;             // +0x12c
    int field_0x134;                    // +0x134
    float field_0x138;                  // +0x138
    uint8_t field_0x13c;                // +0x13c
    int field_0x140;                    // +0x140
    int field_0x144;                    // +0x144
    int field_0x148;                    // +0x148
    uint8_t field_0x14c;                // +0x14c
    int field_0x150;                    // +0x150
    int field_0x158;                    // +0x158
    int field_0x15c;                    // +0x15c
    float field_0x160;                  // +0x160
    int field_0x164;                    // +0x164
    int field_0x168;                    // +0x168
    int field_0x16c;                    // +0x16c
};
#endif
