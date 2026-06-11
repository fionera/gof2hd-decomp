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

// Local matrix payload (this+0x13c, 0x3c = 60 bytes), returned by value from getLocal().
struct Mat60 { float m[15]; };

struct TargetFollowCamera {
    unsigned id;                 // +0x0
    void* target;                    // +0x4
    float posX;                    // +0x8
    float posY;                    // +0xc
    float posZ;                   // +0x10
    float targetX;                   // +0x14
    float targetY;                   // +0x18
    float targetZ;                   // +0x1c
    int upX;                     // +0x20
    int upY;                     // +0x24
    int upZ;                     // +0x28
    int targetOffsetX;                     // +0x2c
    int targetOffsetY;                     // +0x30
    int targetOffsetZ;                     // +0x34
    int camOffsetX;                     // +0x38
    int camOffsetY;                     // +0x3c
    int camOffsetZ;                     // +0x40
    uint8_t locked;                 // +0x44
    uint8_t lookAtCam;                 // +0x45
    uint8_t active;                 // +0x46
    uint8_t rumbleActive;                 // +0x47
    int rumbleTimer;                     // +0x48
    uint8_t rotateAroundTarget;                 // +0x4c
    uint8_t fastForward;                 // +0x4d
    int rotX;                     // +0x50
    int rotY;                     // +0x54
    int rotZ;                     // +0x58
    float zoom;                   // +0xb0
    char firstPerson;                    // +0xf0
    int fpOffsetX;                     // +0xf4
    int fpOffsetY;                     // +0xf8
    int fpOffsetZ;                     // +0xfc
    uint8_t hideShip;                // +0x100
    float shakeAccum;                  // +0x104
    float shakeReference;                  // +0x108
    uint8_t useTargetsUpVector;                // +0x10c
    float shakeAmount;                  // +0x110
    int shakeFrequency;                    // +0x114
    int rumbleStrength;                    // +0x120
    uint8_t field_0x124;                // +0x124
    float handlingDampingA;                  // +0x128
    float handlingDampingB;                  // +0x12c
    float roll;                  // +0x130
    float shipHandling;                  // +0x134
    uint8_t fixed;                // +0x138
    Mat60 localMatrix;                  // +0x13c
};
#endif
