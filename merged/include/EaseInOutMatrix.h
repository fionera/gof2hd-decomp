#ifndef WORK_CLASSES_EASEINOUTMATRIX_SRC_CLASS_H
#define WORK_CLASSES_EASEINOUTMATRIX_SRC_CLASS_H

namespace AbyssEngine {

namespace AEMath {
struct Matrix {
    float m[15];
};
}

struct Quaternion {
    float x, y, z, w;
    ~Quaternion();
};

// Layout (byte offsets):
//   0x00  Matrix  m_min      (15 floats / 60 bytes)
//   0x3c  Quaternion m_q0
//   0x58  Quaternion m_q1
//   0x74  float   m_t        (eased parameter; runs 0.75 .. 1.25, rests at 1.0)
//   0x78  Matrix  m_current
//   0xb4  Matrix  m_max
//   0xf0  float   m_duration
struct EaseInOutMatrix {
    AEMath::Matrix GetMinValue();
    AEMath::Matrix GetValue();
    AEMath::Matrix GetMaxValue();

    void SetToMinValue();
    void SetToMaxValue();
    void SetDuration(int duration);

    void Increase(float dt);
    void Decrease(float dt);
    void RunOut(float dt);
    void UpdateCurrentValue();

    ~EaseInOutMatrix();
};

static inline float &f32(EaseInOutMatrix *self, unsigned off) {
    return *(float *)((char *)self + off);
}

} // namespace AbyssEngine

#endif
