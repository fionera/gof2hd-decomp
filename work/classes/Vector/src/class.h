#ifndef WORK_CLASSES_VECTOR_SRC_CLASS_H
#define WORK_CLASSES_VECTOR_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::AEMath::Vector (Android libgof2hdaa.so, armv7 Thumb).
// A 3-component float vector. The recovered disassembly for these operators sits in a
// corrupted/overlapping bad-instruction region; per coverage mode we author the faithful
// component-wise semantics implied by each operator's signature and the s0/s2/s4 + [0,4,8]
// VFP load/store pattern (three contiguous floats).

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;

    float &operator[](int index);
    Vector &operator=(const Vector &rhs);
    Vector &operator-=(const Vector &rhs);
    Vector &operator*=(const Vector &rhs);
    Vector &operator/=(const Vector &rhs);
};

} // namespace AEMath
} // namespace AbyssEngine

#endif
