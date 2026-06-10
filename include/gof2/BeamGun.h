#ifndef GOF2_BEAMGUN_H
#define GOF2_BEAMGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- BeamGun (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level "BeamGun::..." symbols. Field offsets are recovered
// per method and accessed directly instead of modeling a full class layout.


struct BeamGun;
struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct AEGeometry;
struct PaintCanvas;
struct Transform;
struct Array;

namespace AbyssEngine {
namespace AEMath {





} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

struct BeamGun {
    void* field_0x0;                    // +0x0
    int32_t field_0x4;                  // +0x4
    Gun* field_0x8;                     // +0x8
    Level* field_0xc;                   // +0xc
    int32_t field_0x10;                 // +0x10
    int32_t field_0x14;                 // +0x14
    AEGeometry* field_0x18;             // +0x18
    AEGeometry* field_0x1c;             // +0x1c
    uint8_t field_0x20;                 // +0x20
    uint8_t field_0x21;                 // +0x21
};
#endif
