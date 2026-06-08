#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(Matrix mn, Matrix mx, int duration)
//   As the default constructor, but seeds the range from the supplied matrices
//   and converts the integer duration to its float storage.
EaseInOutMatrix::EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration)
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

    SetRange(mn, mx);
    f32(this, 0xf0) = (float)duration;
}

} // namespace AbyssEngine
