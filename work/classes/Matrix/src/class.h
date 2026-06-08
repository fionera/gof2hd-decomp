#ifndef WORK_CLASSES_MATRIX_SRC_CLASS_H
#define WORK_CLASSES_MATRIX_SRC_CLASS_H

// Galaxy on Fire 2 -- AbyssEngine::AEMath::Matrix (Android libgof2hdaa.so, armv7 Thumb).
// (Confirmed namespace from the work-item Sig: AbyssEngine::AEMath::Matrix.)
//
// Layout: a 3x4 row-major affine matrix (12 floats, offsets 0x00..0x2c) followed by an
// embedded AEMath::Vector at +0x30. The 4th column (0x0c, 0x1c, 0x2c) is translation.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
    Vector &operator=(const Vector &other);
    Vector &operator*=(const Vector &other);
};

struct Matrix {
    float m[12];      // 0x00..0x2c : rows of 4 (last col = translation)
    Vector pos;       // 0x30       : embedded vector

    Matrix();
    Matrix &operator=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);
};

} // namespace AEMath
} // namespace AbyssEngine

#endif
