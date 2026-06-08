#ifndef WORK_CLASSES_SPACEPOINT_SRC_CLASS_H
#define WORK_CLASSES_SPACEPOINT_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};
} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;

struct SpacePoint;

// Field accessor via byte offset.
template <class T>
static inline T &F(void *self, unsigned off) { return *(T *)((char *)self + off); }

#endif
