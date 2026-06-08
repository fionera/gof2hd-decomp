#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::EaseInOutMatrix() -- default constructor.
//   Default-constructs the embedded matrices/quaternions, then initialises the
//   range to identity .. identity with a zero duration.
EaseInOutMatrix::EaseInOutMatrix()
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

    AEMath::Matrix ident;   // AEMath::Matrix() yields the identity 3x5 matrix.
    SetRange(ident, ident);
    this->f_f0 = 0.0f;
}

} // namespace AbyssEngine
