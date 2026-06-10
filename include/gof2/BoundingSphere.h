#ifndef GOF2_BOUNDINGSPHERE_H
#define GOF2_BOUNDINGSPHERE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
namespace AbyssEngine {
namespace AEMath {



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

struct BoundingSphere { void* _opaque; };  // no offset accesses observed
#endif
