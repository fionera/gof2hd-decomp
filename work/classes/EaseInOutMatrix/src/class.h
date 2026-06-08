#ifndef WORK_CLASSES_EASEINOUTMATRIX_SRC_CLASS_H
#define WORK_CLASSES_EASEINOUTMATRIX_SRC_CLASS_H

typedef unsigned int uint32_t;
inline void *operator new(uint32_t, void *p) noexcept { return p; }

namespace AbyssEngine {

struct EaseInOutMatrix;

namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix {
    float m[15];
    Matrix();
    Matrix &operator=(const Matrix &o);
};
float Sinf(float x);
Vector MatrixGetPosition(const Matrix &m);
void MatrixSetTranslation(Matrix *m, Vector *v);
Vector operator*(const Vector &v, float s);
Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
} // namespace AEMath

struct Quaternion {
    float x, y, z, w;
    Quaternion();
    Quaternion(const AEMath::Matrix &m);
    void Set(const AEMath::Matrix &m);
    void Convert(AEMath::Matrix *out);
    ~Quaternion();
};
Quaternion operator*(const Quaternion &q, float s);
Quaternion operator+(const Quaternion &a, const Quaternion &b);
Quaternion operator-(const Quaternion &a, const Quaternion &b);

// Layout (byte offsets):
//   0x00  Matrix  m_min      (15 floats / 60 bytes)
//   0x3c  Quaternion m_q0
//   0x58  Quaternion m_q1
//   0x74  float   m_t        (eased parameter; runs 0.75 .. 1.25, rests at 1.0)
//   0x78  Matrix  m_current
//   0xb4  Matrix  m_max
//   0xf0  float   m_duration
struct EaseInOutMatrix {
    EaseInOutMatrix();
    EaseInOutMatrix(AEMath::Matrix mn, AEMath::Matrix mx, int duration);

    AEMath::Matrix GetMinValue();
    AEMath::Matrix GetValue();
    AEMath::Matrix GetMaxValue();

    void SetToMinValue();
    void SetToMaxValue();
    void SetDuration(int duration);
    void SetRange(AEMath::Matrix mn, AEMath::Matrix mx);

    void Increase(float dt);
    void Decrease(float dt);
    void RunOut(float dt);
    void UpdateCurrentValue();

    ~EaseInOutMatrix();
};

static inline float &f32(EaseInOutMatrix *self, unsigned off) {
    return *(float *)((char *)self + off);
}

static inline uint32_t &u32(EaseInOutMatrix *self, unsigned off) {
    return *(uint32_t *)((char *)self + off);
}

} // namespace AbyssEngine

#endif
