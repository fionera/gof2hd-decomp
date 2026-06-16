#include "engine/math/EaseInOutMatrix.h"
#include <algorithm>
#include <new>

namespace AbyssEngine {

void EaseInOutMatrix::RunOut(float dt) {
    float target = 1.0f;
    float t = m_t;
    if (t > target) {
        t = t + (dt * -0.5f) / m_duration;
        m_t = t;
        if (t < target) {
            m_t = 1.0f;
        }
    } else if (t < target) {
        t = t + (dt * 0.5f) / m_duration;
        m_t = t;
        if (t > target) {
            m_t = 1.0f;
        }
    }
    UpdateCurrentValue();
}

AEMath::Matrix EaseInOutMatrix::GetValue() {
    return this->m_current;
}

EaseInOutMatrix::~EaseInOutMatrix() = default;

void EaseInOutMatrix::SetDuration(int duration) {
    m_duration = (float)duration;
}

AEMath::Matrix EaseInOutMatrix::GetMaxValue() {
    return this->m_max;
}

AEMath::Matrix EaseInOutMatrix::GetMinValue() {
    return this->m_min;
}

void EaseInOutMatrix::SetToMaxValue() {
    m_t = 1.25f;
    UpdateCurrentValue();
}

// Stores the two end matrices, derives the start/end orientation quaternions
//   and the translation delta, parks the eased parameter at 0.75 (the min end),
//   and refreshes the current matrix.
void EaseInOutMatrix::SetRange(AEMath::Matrix mn, AEMath::Matrix mx)
{
    m_min = mn;
    m_max = mx;

    m_q0.Set(mn);

    // Min translation.
    AEMath::Vector minPos = AEMath::MatrixGetPosition(mn);
    this->m_minPos = minPos;

    // Orientation delta q1 = q(max) - q0.
    Quaternion qMax(mx);
    m_q1 = qMax - m_q0;

    // Translation delta = pos(max) - pos(min).
    AEMath::Vector maxPos = AEMath::MatrixGetPosition(mx);
    this->m_posDelta = maxPos - minPos;

    this->m_t = 0.75f;
    UpdateCurrentValue();
}

// Recomputes the current matrix from the eased parameter t. At t == 1.25 the
//   current matrix is exactly the max matrix; otherwise the rotation is the
//   slerp-style blend of the two stored quaternions and the translation is the
//   min translation plus the eased delta.
void EaseInOutMatrix::UpdateCurrentValue()
{
    AEMath::Matrix &current = this->m_current;

    if (this->m_t == 1.25f) {
        current = this->m_max;
        return;
    }

    static const float kSweep = 3.14159265f;
    float s = AEMath::Sinf(this->m_t * kSweep);
    float w = s * 0.5f + 0.5f;

    Quaternion blended = m_q1 * w + m_q0;
    blended.Convert(current);

    // Translation: min translation plus eased delta.
    AEMath::Vector t = this->m_minPos + this->m_posDelta * w;
    AEMath::MatrixSetTranslation(current, t);
}

void EaseInOutMatrix::Increase(float dt) {
    float t = this->m_t + (dt * 0.5f) / this->m_duration;
    this->m_t = std::min(t, 1.25f);
    UpdateCurrentValue();
}

void EaseInOutMatrix::SetToMinValue() {
    this->m_t = 0.75f;
    UpdateCurrentValue();
}

void EaseInOutMatrix::Decrease(float dt) {
    float t = this->m_t + (dt * -0.5f) / this->m_duration;
    this->m_t = std::max(t, 0.75f);
    UpdateCurrentValue();
}

EaseInOutMatrix::EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration)
{
    this->m_minPos = AEMath::Vector{0, 0, 0};
    this->m_posDelta = AEMath::Vector{0, 0, 0};

    SetRange(mn, mx);
    this->m_duration = (float)duration;
}

// Default constructor: initialises the range to identity .. identity with a zero duration.
EaseInOutMatrix::EaseInOutMatrix()
{
    this->m_minPos = AEMath::Vector{0, 0, 0};
    this->m_posDelta = AEMath::Vector{0, 0, 0};

    AEMath::Matrix ident;
    AEMath::MatrixIdentity(ident);
    SetRange(ident, ident);
    this->m_duration = 0.0f;
}

} // namespace AbyssEngine

// ---- C-ABI shims (recovered) ----
// These are the C-callable faces of the real EaseInOutMatrix constructor and
// destructor; consuming translation units (SpaceLounge, PlayerEgo, ModStation,
// PlayerFighter, ...) reach the object through these flat entry points.

// EaseInOutMatrix_ctor — in-place construct an EaseInOutMatrix from two camera
// matrices and an animation duration (ms). `from`/`to` point at 0x3c-byte
// Matrix blobs on the caller's stack.
extern "C" void EaseInOutMatrix_ctor(void *ease, void *from, void *to, int duration)
{
    new (ease) AbyssEngine::EaseInOutMatrix(
        *(AbyssEngine::AEMath::Matrix *)from,
        *(AbyssEngine::AEMath::Matrix *)to,
        duration);
}

// EaseInOutMatrix_dtor — non-deleting destructor. Runs ~EaseInOutMatrix() in
// place (releasing the two orientation quaternions) and returns the storage so
// the caller can hand it to operator delete.
extern "C" void *EaseInOutMatrix_dtor(void *p)
{
    if (p) ((AbyssEngine::EaseInOutMatrix *)p)->~EaseInOutMatrix();
    return p;
}
