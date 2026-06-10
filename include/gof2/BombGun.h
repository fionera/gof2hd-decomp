#ifndef GOF2_BOMBGUN_H
#define GOF2_BOMBGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct BombGun;
struct RocketGun;
struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct LevelScript;
struct Explosion;
struct AEGeometry;
struct PaintCanvas;
struct Transform;
struct TargetFollowCamera;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {





} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;





typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

struct BombGun {
    Gun* field_0x8;                     // +0x8
    uint32_t field_0x10;                // +0x10
    uint32_t field_0x14;                // +0x14
    float field_0x20;                   // +0x20
    uint8_t field_0x24;                 // +0x24
    uint16_t field_0x28;                // +0x28
    AEGeometry* field_0xe8;             // +0xe8
    PlayerEgo* field_0xec;              // +0xec
    Explosion* field_0xf0;              // +0xf0
    uint32_t field_0xf4;                // +0xf4
    volatile uint64_t field_0xf8;       // +0xf8
    volatile int field_0x100;           // +0x100
    uint8_t field_0x104;                // +0x104
    int field_0x108;                    // +0x108
    float field_0x10c;                  // +0x10c
    v4i field_0x110;                    // +0x110
    volatile uint64_t field_0x120;      // +0x120
    int field_0x128;                    // +0x128

    void setPlayer(PlayerEgo *player);
    void render();
    void update(int elapsed);
};
#endif
