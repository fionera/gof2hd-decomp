#ifndef WORK_CLASSES_TARGETFOLLOWCAMERA_SRC_CLASS_H
#define WORK_CLASSES_TARGETFOLLOWCAMERA_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

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
template <class T>
static inline T &F(TargetFollowCamera *self, unsigned off) { return *(T *)((char *)self + off); }

namespace AbyssEngine {
namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
}
}
using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

#endif
