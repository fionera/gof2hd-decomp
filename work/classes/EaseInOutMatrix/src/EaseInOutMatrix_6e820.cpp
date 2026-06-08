#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::EaseInOutMatrix() -- default constructor.
//   Default-constructs the embedded matrices/quaternions, then initialises the
//   range to identity .. identity with a zero duration.
EaseInOutMatrix::EaseInOutMatrix()
{
    new ((void *)((char *)this + 0x00)) AEMath::Matrix();
    new ((void *)((char *)this + 0x3c)) Quaternion();
    u32(this, 0x4c) = 0;
    u32(this, 0x50) = 0;
    u32(this, 0x54) = 0;
    new ((void *)((char *)this + 0x58)) Quaternion();
    u32(this, 0x68) = 0;
    u32(this, 0x6c) = 0;
    u32(this, 0x70) = 0;
    new ((void *)((char *)this + 0x78)) AEMath::Matrix();
    new ((void *)((char *)this + 0xb4)) AEMath::Matrix();

    AEMath::Matrix ident;   // AEMath::Matrix() yields the identity 3x5 matrix.
    SetRange(ident, ident);
    f32(this, 0xf0) = 0.0f;
}

} // namespace AbyssEngine
