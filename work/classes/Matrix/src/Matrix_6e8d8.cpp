#include "class.h"

// AbyssEngine::AEMath::Matrix::Matrix()
// Initializes to the identity affine transform. The diagonal entries (0x00, 0x14, 0x38)
// are 1.0; the remaining rotation/translation entries are zeroed, and the trailing
// pair (0x28..0x34) is loaded from a constant {0,0,1,0} table.

namespace AbyssEngine {
namespace AEMath {

// Constant {0,0,1,0} pair the original loads via PC-relative literals.
static const float kIdentTail[4] = { 0.0f, 0.0f, 1.0f, 0.0f };

Matrix::Matrix()
{
    float *m = (float *)this;
    m[0] = 1.0f;   // 0x00
    m[5] = 1.0f;   // 0x14
    *(float *)((char *)this + 0x38) = 1.0f;

    m[1] = 0.0f;   // 0x04
    m[2] = kIdentTail[1];   // 0x08
    m[3] = kIdentTail[2];   // 0x0c
    m[4] = kIdentTail[3];   // 0x10

    m[6] = 0.0f;   // 0x18
    m[7] = kIdentTail[1];   // 0x1c
    m[8] = kIdentTail[2];   // 0x20
    m[9] = kIdentTail[3];   // 0x24

    *(float *)((char *)this + 0x28) = kIdentTail[0];
    *(float *)((char *)this + 0x2c) = kIdentTail[1];
    *(float *)((char *)this + 0x30) = kIdentTail[2];
    *(float *)((char *)this + 0x34) = kIdentTail[3];
}

} // namespace AEMath
} // namespace AbyssEngine
