#ifndef GOF2_TARGETFOLLOWCAMERA_H
#define GOF2_TARGETFOLLOWCAMERA_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — TargetFollowCamera class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: namespace); camera math uses AbyssEngine::AEMath
// Vector/Matrix. Field offsets recovered from each per-method target disassembly.
// We do NOT model a full layout: methods access fields via byte-offset casts from
// `this` (which arrives in r0 just like the first explicit arg of an extern "C" fn).
//
// The verify harness (tools/gofdiff.py) matches the built symbol by *substring* of
// the demangled name, so we author each method as an extern "C" function whose name
// contains the target method name (e.g. TFC_setActive). This sidesteps C++ mangling
// while still producing the exact target code.


struct TargetFollowCamera;

// Known field offsets (from per-method disasm):
//   0x08 m_nPosX  0x0c m_nPosY  0x10 m_nPosZ   (Vector position)
//   0x44 bool active?  0x45 lookAtCam bool  0x46 active bool  0x4c rotAroundTarget bool
//   0x4d fastForward bool
//   0xb0 zoom/camOffset-length float
//   0x100 hideShip bool  0x10c useTargetsUpVector bool
//   0x110 m_nShakeAmount float  0x114 m_nShakeFrequency int
//   0x128/0x12c ship handling (float,float)  0x130 roll float
//   0x13c local matrix (Matrix, 0x3c bytes)
//   0xb4 firstPerson matrix

// Field accessor via byte offset.


namespace AbyssEngine {
namespace AEMath {


}
}
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

struct TargetFollowCamera {
    unsigned field_0x0;                 // +0x0
    void* field_0x4;                    // +0x4
    float field_0x8;                    // +0x8
    float field_0xc;                    // +0xc
    float field_0x10;                   // +0x10
    float field_0x14;                   // +0x14
    float field_0x18;                   // +0x18
    float field_0x1c;                   // +0x1c
    int field_0x20;                     // +0x20
    int field_0x24;                     // +0x24
    int field_0x28;                     // +0x28
    int field_0x2c;                     // +0x2c
    int field_0x30;                     // +0x30
    int field_0x34;                     // +0x34
    int field_0x38;                     // +0x38
    int field_0x3c;                     // +0x3c
    int field_0x40;                     // +0x40
    uint8_t field_0x44;                 // +0x44
    uint8_t field_0x45;                 // +0x45
    uint8_t field_0x46;                 // +0x46
    uint8_t field_0x47;                 // +0x47
    int field_0x48;                     // +0x48
    uint8_t field_0x4c;                 // +0x4c
    uint8_t field_0x4d;                 // +0x4d
    int field_0x50;                     // +0x50
    int field_0x54;                     // +0x54
    int field_0x58;                     // +0x58
    float field_0xb0;                   // +0xb0
    char field_0xf0;                    // +0xf0
    int field_0xf4;                     // +0xf4
    int field_0xf8;                     // +0xf8
    int field_0xfc;                     // +0xfc
    uint8_t field_0x100;                // +0x100
    float field_0x104;                  // +0x104
    float field_0x108;                  // +0x108
    uint8_t field_0x10c;                // +0x10c
    float field_0x110;                  // +0x110
    int field_0x114;                    // +0x114
    int field_0x120;                    // +0x120
    uint8_t field_0x124;                // +0x124
    float field_0x128;                  // +0x128
    float field_0x12c;                  // +0x12c
    float field_0x130;                  // +0x130
    float field_0x134;                  // +0x134
    uint8_t field_0x138;                // +0x138
    Mat60 field_0x13c;                  // +0x13c
};
#endif
