#ifndef GOF2_MATRIX_H
#define GOF2_MATRIX_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine::AEMath::Matrix (Android libgof2hdaa.so, armv7 Thumb).
// (Confirmed namespace from the work-item Sig: AbyssEngine::AEMath::Matrix.)
//
// Layout: a 3x4 row-major affine matrix (12 floats, offsets 0x00..0x2c) followed by an
// embedded AEMath::Vector at +0x30. The 4th column (0x0c, 0x1c, 0x2c) is translation.
// Field offsets recovered from the target disassembly; accessed via byte-offset casts.


namespace AbyssEngine {
namespace AEMath {





} // namespace AEMath
} // namespace AbyssEngine

struct Matrix {
    float field_0x28;                   // +0x28
    float field_0x2c;                   // +0x2c
    float field_0x30;                   // +0x30
    float field_0x34;                   // +0x34
    float field_0x38;                   // +0x38
};
#endif
