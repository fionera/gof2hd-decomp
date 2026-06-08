#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::EaseInOutMatrix(Matrix mn, Matrix mx, int duration)
//   As the default constructor, but seeds the range from the supplied matrices
//   and converts the integer duration to its float storage.
EaseInOutMatrix::EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration)
{
    new ((void *)((char *)this + 0x00)) AEMath::Matrix();
    new ((void *)((char *)this + 0x3c)) Quaternion();
    this->f_4c = 0;
    this->f_50 = 0;
    this->f_54 = 0;
    new ((void *)((char *)this + 0x58)) Quaternion();
    this->f_68 = 0;
    this->f_6c = 0;
    this->f_70 = 0;
    new ((void *)((char *)this + 0x78)) AEMath::Matrix();
    new ((void *)((char *)this + 0xb4)) AEMath::Matrix();

    SetRange(mn, mx);
    this->f_f0 = (float)duration;
}

} // namespace AbyssEngine
