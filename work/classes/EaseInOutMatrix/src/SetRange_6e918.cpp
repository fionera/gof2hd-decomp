#include "class.h"

namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::SetRange(Matrix mn, Matrix mx)
//   Stores the two end matrices, derives the start/end orientation quaternions
//   and the translation delta, parks the eased parameter at 0.75 (the min end),
//   and refreshes the current matrix.
void EaseInOutMatrix::SetRange(AEMath::Matrix mn, AEMath::Matrix mx)
{
    AEMath::Matrix *minM = (AEMath::Matrix *)((char *)this + 0x00);
    AEMath::Matrix *maxM = (AEMath::Matrix *)((char *)this + 0xb4);
    *minM = mn;
    *maxM = mx;

    Quaternion *q0 = (Quaternion *)((char *)this + 0x3c);
    q0->Set(mn);

    // Position of the min matrix (via an identity carrier the original builds).
    AEMath::Matrix ident;
    q0->Convert(&ident);
    Quaternion qIdent(ident);
    AEMath::Vector minPos = AEMath::MatrixGetPosition(mn);
    *(AEMath::Vector *)((char *)this + 0x4c) = minPos;

    // Orientation delta q1 = q(max) - q0.
    Quaternion qMax(mx);
    Quaternion *q1 = (Quaternion *)((char *)this + 0x58);
    *q1 = AbyssEngine::operator-(qMax, *q0);

    // Translation delta = pos(max) - pos(min).
    AEMath::Vector maxPos = AEMath::MatrixGetPosition(mx);
    AEMath::Vector dPos = AEMath::operator-(maxPos, minPos);
    *(AEMath::Vector *)((char *)this + 0x68) = dPos;

    f32(this, 0x74) = 0.75f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine
