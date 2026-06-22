#ifndef GOF2_EASEINOUTMATRIX_H
#define GOF2_EASEINOUTMATRIX_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"
#include "engine/math/AEMath.h"

namespace AbyssEngine {
    class EaseInOutMatrix {
    public:
        AEMath::Matrix m_min; // +0x0   start (min) transform
        Quaternion m_q0; // +0x40  start orientation
        AEMath::Vector m_minPos; // +0x4c  start (min) translation
        Quaternion m_q1; // +0x5c  orientation delta
        AEMath::Vector m_posDelta; // +0x68  translation delta
        float m_t; // +0x74  eased parameter
        AEMath::Matrix m_current; // +0x78  current transform
        AEMath::Matrix m_max; // +0xb4  end (max) transform
        float m_duration; // +0xf4

        EaseInOutMatrix();

        EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration);

        ~EaseInOutMatrix();

        void SetRange(AEMath::Matrix mn, AEMath::Matrix mx);

        void SetDuration(int duration);

        void UpdateCurrentValue();

        void RunOut(float dt);

        void Increase(float dt);

        void Decrease(float dt);

        void SetToMinValue();

        void SetToMaxValue();

        AEMath::Matrix GetValue();

        AEMath::Matrix GetMinValue();

        AEMath::Matrix GetMaxValue();
    };

    Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);

    Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);

    Quaternion operator*(const Quaternion &lhs, float rhs);
} // namespace AbyssEngine

#endif
