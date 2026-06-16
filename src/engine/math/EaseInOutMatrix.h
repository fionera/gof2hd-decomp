#ifndef GOF2_EASEINOUTMATRIX_H
#define GOF2_EASEINOUTMATRIX_H
#include "common.h"
#include "mathtypes.h"
#include "engine/math/AEMath.h"
// Eased interpolation between two transform matrices. The eased parameter t
// rests at 1.0 and sweeps over [0.75, 1.25]; the current matrix blends the two
// stored orientation quaternions and translations accordingly.

namespace AbyssEngine {

class EaseInOutMatrix {
public:
    AEMath::Matrix field_0x0;           // start transform (m_min)
    Quaternion     m_q0;                // start orientation
    AEMath::Vector field_0x4c;          // start translation (m_minPos)
    Quaternion     m_q1;                // orientation delta
    AEMath::Vector field_0x68;          // translation delta
    float          m_t;                 // eased parameter
    AEMath::Matrix field_0x78;          // current transform
    AEMath::Matrix field_0xb4;          // end transform (m_max)
    float          m_duration;

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

// Quaternion free operators (defined in the merged Quaternion.cpp).
Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, float rhs);

} // namespace AbyssEngine

#endif
