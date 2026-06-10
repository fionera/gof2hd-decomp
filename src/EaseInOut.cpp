#include "gof2/EaseInOut.h"




// ---- UpdateCurrentValue_717a8.cpp ----
namespace AbyssEngine {

void EaseInOut::UpdateCurrentValue() {
    if ((double)m_t == 2.5 * PI) {
        m_current = m_min + m_range;
    } else {
        double s = (double)AEMath::Sinf(m_t) * 0.5 + 0.5;
        m_current = (float)((double)m_min + s * (double)m_range);
    }
}

} // namespace AbyssEngine

// ---- GetMaxValue_719cc.cpp ----
namespace AbyssEngine {

float EaseInOut::GetMaxValue() {
    return m_min + m_range;
}

} // namespace AbyssEngine

// ---- EaseInOut_71810.cpp ----
namespace AbyssEngine {

EaseInOut::EaseInOut(float minValue, float maxValue) {
    SetRange(minValue, maxValue);
}

} // namespace AbyssEngine

// ---- EaseInOut_71780.cpp ----
namespace AbyssEngine {

EaseInOut::EaseInOut() {
    m_min = 0.0f;
    m_range = 2.0f * PI;
    m_t = 1.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- SetToMaxValue_719b0.cpp ----
namespace AbyssEngine {

void EaseInOut::SetToMaxValue() {
    m_t = 2.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- SetRange_71820.cpp ----
namespace AbyssEngine {

void EaseInOut::SetRange(float minValue, float maxValue) {
    m_t = 1.5f * PI;
    m_min = minValue;
    m_range = maxValue - minValue;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- RunOut_71910.cpp ----
namespace AbyssEngine {

void EaseInOut::RunOut(float dt) {
    float target = 2.0f * PI;
    float t = m_t;
    if (t > target) {
        t = (float)((double)t + (double)dt * (-1.0 / 65536.0) * (2.0 * PI));
        m_t = t;
        if (t < target) {
            m_t = target;
        }
    } else if (t < target) {
        t = (float)((double)t + (double)dt * (1.0 / 65536.0) * (2.0 * PI));
        m_t = t;
        if (t > target) {
            m_t = target;
        }
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- SetToMinValue_719bc.cpp ----
namespace AbyssEngine {

void EaseInOut::SetToMinValue() {
    m_t = 1.5f * PI;
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- Decrease_718a8.cpp ----
namespace AbyssEngine {

void EaseInOut::Decrease(float dt) {
    m_t = (float)((double)m_t + (double)dt * (-1.0 / 65536.0) * (2.0 * PI));
    if ((double)m_t < 1.5 * PI) {
        m_t = 1.5f * PI;
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine

// ---- Increase_71840.cpp ----
namespace AbyssEngine {

void EaseInOut::Increase(float dt) {
    m_t = (float)((double)m_t + (double)dt * (1.0 / 65536.0) * (2.0 * PI));
    if ((double)m_t > 2.5 * PI) {
        m_t = 2.5f * PI;
    }
    UpdateCurrentValue();
}

} // namespace AbyssEngine
