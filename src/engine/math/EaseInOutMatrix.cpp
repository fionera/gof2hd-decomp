#include "engine/math/EaseInOutMatrix.h"
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

    void EaseInOutMatrix::SetDuration(int duration) {
        m_duration = (float) duration;
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

    void EaseInOutMatrix::SetRange(AEMath::Matrix mn, AEMath::Matrix mx) {
        m_min = mn;
        m_max = mx;

        m_q0.Set(mn);
        AEMath::Matrix check;
        m_q0.Convert(check);
        Quaternion roundTrip;
        roundTrip.Set(check);

        m_minPos = AEMath::MatrixGetPosition(mn);
        m_q1 = Quaternion(mx) - m_q0;
        m_posDelta = AEMath::MatrixGetPosition(mx) - m_minPos;

        m_t = 0.75f;
        UpdateCurrentValue();
    }

    void EaseInOutMatrix::UpdateCurrentValue() {
        if (m_t == 1.25f) {
            m_current = m_max;
            return;
        }

        float w = (float) (AEMath::Sinf(m_t * 6.2831855f) * 0.5 + 0.5);
        (m_q1 * w + m_q0).Convert(m_current);
        AEMath::MatrixSetTranslation(m_current, m_posDelta * w + m_minPos);
    }

    void EaseInOutMatrix::Increase(float dt) {
        float t = m_t + (dt * 0.5f) / m_duration;
        m_t = t > 1.25f ? 1.25f : t;
        UpdateCurrentValue();
    }

    void EaseInOutMatrix::SetToMinValue() {
        this->m_t = 0.75f;
        UpdateCurrentValue();
    }

    void EaseInOutMatrix::Decrease(float dt) {
        float t = m_t + (dt * -0.5f) / m_duration;
        m_t = t < 0.75f ? 0.75f : t;
        UpdateCurrentValue();
    }

    EaseInOutMatrix::EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration)
        : m_minPos(), m_posDelta() {
        SetRange(mn, mx);
        m_duration = (float) duration;
    }

    EaseInOutMatrix::EaseInOutMatrix()
        : m_minPos(), m_posDelta() {
        SetRange(AEMath::Matrix(), AEMath::Matrix());
        m_duration = 0.0f;
    }
}

