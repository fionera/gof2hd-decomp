#ifndef GOF2_SPACEPOINT_H
#define GOF2_SPACEPOINT_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 — SpacePoint class. Top-level (no AbyssEngine:: on the class
// itself), but its constructor stores AbyssEngine::AEMath::Vector members via
// Vector::operator=. Field offsets recovered from per-method target disassembly.
//
// Known fields:
//   +0x00  Vector position   (3 floats; via Vector::operator=)
//   +0x0c  Vector direction  (3 floats; via Vector::operator=)
//   +0x10  int   (zeroed in ctor)
//   +0x14  int   (zeroed in ctor)
//   +0x18  int   type/param_1
//   +0x1c  bool  free/taken flag
//   +0x20  int   param_4
//
// The verify harness (tools/gofdiff.py) matches the built symbol by *substring*
// of the demangled name, so each method is authored as an extern "C" function
// whose name contains the target method name. This sidesteps C++ mangling.


namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;

struct SpacePoint;

// Field accessor via byte offset.

class SpacePoint {
public:
    Vector position;                    // +0x00
    Vector direction;                   // +0x0c
    int field_0x10;                     // +0x10
    int field_0x14;                     // +0x14
    int type;                     // +0x18
    uint8_t free;                 // +0x1c
    int param;                     // +0x20

    SpacePoint(int param_1, const Vector &param_2, const Vector &param_3, int param_4);

    // ---- methods (converted from free functions) ----
    void giveFree();
    void take();
};
#endif
