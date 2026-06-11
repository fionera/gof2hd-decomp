#ifndef GOF2_EASEINOUTMATRIX_H
#define GOF2_EASEINOUTMATRIX_H
#include "gof2/common.h"
#include "gof2/math.h"
#include "gof2/AEMath.h"
// Galaxy on Fire 2 - AbyssEngine::EaseInOutMatrix.
// Eased interpolation between two transform matrices. Real named struct
// (the deterministic field_0xNN names are kept as member names so cross-class
// references resolve), no byte-offset access.
//
// Original byte layout (ARM32 target):
//   0x00  Matrix     field_0x0    m_min      (start transform)
//   0x3c  Quaternion m_q0         start orientation
//   0x4c  Vector     field_0x4c   m_minPos   start translation
//   0x58  Quaternion m_q1         orientation delta (max - min)
//   0x68  Vector     field_0x68   m_deltaPos translation delta (max - min)
//   0x74  float      m_t          eased parameter (0.75 .. 1.25, rests at 1.0)
//   0x78  Matrix     field_0x78   m_current  current eased transform
//   0xb4  Matrix     field_0xb4   m_max      end transform
//   0xf0  float      m_duration

namespace AbyssEngine {

class EaseInOutMatrix {
public:
    AEMath::Matrix field_0x0;           // +0x0   start transform (m_min)
    Quaternion     m_q0;                // +0x3c  start orientation
    AEMath::Vector field_0x4c;          // +0x4c  start translation (m_minPos)
    Quaternion     m_q1;                // +0x58  orientation delta
    AEMath::Vector field_0x68;          // +0x68  translation delta
    float          m_t;                 // +0x74  eased parameter
    AEMath::Matrix field_0x78;          // +0x78  current transform
    AEMath::Matrix field_0xb4;          // +0xb4  end transform (m_max)
    float          m_duration;          // +0xf0

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
