#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::UpdateCurrentValue()
//   Recomputes the current matrix (+0x78) from the eased parameter t (+0x74).
//   At t == 1.25 the current matrix is exactly the max matrix; otherwise the
//   rotation is the slerp-style blend of the two stored quaternions and the
//   translation is the min translation plus the eased delta.
void EaseInOutMatrix::UpdateCurrentValue()
{
    AEMath::Matrix *current = (AEMath::Matrix *)((char *)this + 0x78);

    if (f32(this, 0x74) == 1.25f) {
        *current = *(AEMath::Matrix *)((char *)this + 0xb4);
        return;
    }

    // DAT_0007ed00 is the sweep constant applied to the eased parameter.
    static const float kSweep = 3.14159265f;
    float s = AEMath::Sinf(f32(this, 0x74) * kSweep);
    float w = s * 0.5f + 0.5f;

    Quaternion *q0 = (Quaternion *)((char *)this + 0x3c);
    Quaternion *q1 = (Quaternion *)((char *)this + 0x58);
    Quaternion blended = AbyssEngine::operator+(AbyssEngine::operator*(*q1, w), *q0);
    blended.Convert(current);

    // Translation: min translation (+0x4c) plus eased delta (+0x68).
    AEMath::Vector *minT = (AEMath::Vector *)((char *)this + 0x4c);
    AEMath::Vector *delta = (AEMath::Vector *)((char *)this + 0x68);
    AEMath::Vector t = AEMath::operator+(*minT, AEMath::operator*(*delta, w));
    AEMath::MatrixSetTranslation(current, &t);
}

} // namespace AbyssEngine
