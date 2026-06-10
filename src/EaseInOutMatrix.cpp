#include "gof2/EaseInOutMatrix.h"
#include <arm_neon.h>



// ---- RunOut_6ed60.cpp ----
namespace AbyssEngine {

void EaseInOutMatrix::RunOut(float dt) {
    float target = 1.0f;
    float t = f32(this, 0x74);
    if (t > target) {
        t = t + (dt * -0.5f) / f32(this, 0xf0);
        f32(this, 0x74) = t;
        if (t < target) {
            f32(this, 0x74) = 1.0f;
        }
    } else if (t < target) {
        t = t + (dt * 0.5f) / f32(this, 0xf0);
        f32(this, 0x74) = t;
        if (t > target) {
            f32(this, 0x74) = 1.0f;
        }
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- GetValue_6edcc.cpp ----
namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetValue() {
    return this->field_0x78;
}

} // namespace AbyssEngine

// ---- _EaseInOutMatrix_9a1f6.cpp ----
namespace AbyssEngine {

EaseInOutMatrix::~EaseInOutMatrix() {
    ((Quaternion *)((char *)this + 0x58))->~Quaternion();
    ((Quaternion *)((char *)this + 0x3c))->~Quaternion();
}

} // namespace AbyssEngine

// ---- SetDuration_6ead4.cpp ----
namespace AbyssEngine {

void EaseInOutMatrix::SetDuration(int duration) {
    f32(this, 0xf0) = (float)duration;
}

} // namespace AbyssEngine

// ---- GetMaxValue_6edfe.cpp ----
namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetMaxValue() {
    return this->field_0xb4;
}

} // namespace AbyssEngine

// ---- GetMinValue_6edf2.cpp ----
namespace AbyssEngine {

AEMath::Matrix EaseInOutMatrix::GetMinValue() {
    return this->field_0x0;
}

} // namespace AbyssEngine

// ---- SetToMaxValue_6eddc.cpp ----
namespace AbyssEngine {

void EaseInOutMatrix::SetToMaxValue() {
    f32(this, 0x74) = 1.25f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- SetRange_6e918.cpp ----
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
    this->field_0x4c = minPos;

    // Orientation delta q1 = q(max) - q0.
    Quaternion qMax(mx);
    Quaternion *q1 = (Quaternion *)((char *)this + 0x58);
    *q1 = AbyssEngine::operator-(qMax, *q0);

    // Translation delta = pos(max) - pos(min).
    AEMath::Vector maxPos = AEMath::MatrixGetPosition(mx);
    AEMath::Vector dPos = AEMath::operator-(maxPos, minPos);
    this->field_0x68 = dPos;

    this->f_74 = 0.75f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- UpdateCurrentValue_6ec1c.cpp ----
namespace AbyssEngine {

// AbyssEngine::EaseInOutMatrix::UpdateCurrentValue()
//   Recomputes the current matrix (+0x78) from the eased parameter t (+0x74).
//   At t == 1.25 the current matrix is exactly the max matrix; otherwise the
//   rotation is the slerp-style blend of the two stored quaternions and the
//   translation is the min translation plus the eased delta.
void EaseInOutMatrix::UpdateCurrentValue()
{
    AEMath::Matrix *current = (AEMath::Matrix *)((char *)this + 0x78);

    if (this->f_74 == 1.25f) {
        *current = this->field_0xb4;
        return;
    }

    // DAT_0007ed00 is the sweep constant applied to the eased parameter.
    static const float kSweep = 3.14159265f;
    float s = AEMath::Sinf(this->f_74 * kSweep);
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

// ---- Increase_6ed08.cpp ----
namespace AbyssEngine {

// NOTE: the target was built with fast-math; its `fminf(t, 1.25f)` lowers to a
// scalar `vmin.f32 d0,d0,d2` + `vstr s0`. Under the fixed -Oz (no -ffast-math)
// harness, clang lowers the equivalent NEON min to vdup/vst1 form instead, so a
// byte-exact match is not reachable with the available flags. Kept best-effort.
void EaseInOutMatrix::Increase(float dt) {
    float t = this->f_74 + (dt * 0.5f) / this->f_f0;
    this->f_74 = vget_lane_f32(vmin_f32(vdup_n_f32(t), vdup_n_f32(1.25f)), 0);
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- SetToMinValue_6ede8.cpp ----
namespace AbyssEngine {

void EaseInOutMatrix::SetToMinValue() {
    this->f_74 = 0.75f;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- Decrease_6ed34.cpp ----
namespace AbyssEngine {

// See Increase: target uses fast-math `fmaxf(t, 0.75f)` -> scalar vmax.f32; not
// byte-reachable under the fixed -Oz harness. Kept best-effort.
void EaseInOutMatrix::Decrease(float dt) {
    float t = this->f_74 + (dt * -0.5f) / this->f_f0;
    this->f_74 = vget_lane_f32(vmax_f32(vdup_n_f32(t), vdup_n_f32(0.75f)), 0);
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- EaseInOutMatrix_6eae2.cpp ----
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

// ---- EaseInOutMatrix_6e820.cpp ----
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
