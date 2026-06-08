#ifndef WORK_CLASSES_BOUNDINGSPHERE_SRC_CLASS_H
#define WORK_CLASSES_BOUNDINGSPHERE_SRC_CLASS_H

typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

    Vector &operator=(const Vector &other);
};

Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator-(const Vector &lhs, const Vector &rhs);
Vector operator*(float lhs, const Vector &rhs);
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorNormalize(const Vector &value);
float VectorLength(const Vector &value);

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct BoundingVolume {
    BoundingVolume(float, float, float, float, float, float);
    ~BoundingVolume();
    bool collide(float, float, float);
    void update(float, float, float);
};

struct BoundingSphere : BoundingVolume {
    BoundingSphere(float, float, float, float, float, float, float);
    ~BoundingSphere();

    bool collide(float, float, float);
    bool outerCollide(float, float, float);
    Vector getCollisionNormal(const Vector &position);
    Vector projectCollisionOnSurface(const Vector &position);
    void update(float, float, float);
};

static inline float &f32(void *self, uint32_t off)
{
    return *(float *)((char *)self + off);
}

static inline void *&pp(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

static inline Vector &vec(void *self, uint32_t off)
{
    return *(Vector *)((char *)self + off);
}

extern "C" void AEMath_VectorAdd(void *out, const void *lhs, const void *rhs);
extern "C" void AEMath_VectorSub(void *out, const void *lhs, const void *rhs);
extern "C" void AEMath_VectorScale(void *out, float scale, const void *value);
extern "C" float AEMath_VectorDot(const void *lhs, const void *rhs);
extern "C" void AEMath_VectorNormalize(void *out, const void *value);
extern "C" float AEMath_VectorLength(const void *value);
extern "C" void *AEMath_VectorAssign(void *out, const void *value);

#endif
