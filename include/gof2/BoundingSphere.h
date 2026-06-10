#ifndef GOF2_BOUNDINGSPHERE_H
#define GOF2_BOUNDINGSPHERE_H
#include "gof2/common.h"
#include "gof2/math.h"
#include "gof2/BoundingVolume.h"

using Vector = AbyssEngine::AEMath::Vector;

// BoundingSphere extends BoundingVolume.
// Layout:
//   0x00..0x1f  BoundingVolume base (vtable, children, center, extents)
//   0x38        float radius
struct BoundingSphere : BoundingVolume {
    // BoundingVolume base occupies 0x00..0x1f (size 0x20). The compiler-laid
    // base is followed by sphere-specific fields. The real binary places the
    // radius at object offset 0x38; padding members keep the layout faithful.
    char field_0x20[0x18];   // padding to reach radius at 0x38
    float field_0x38;        // +0x38 radius

    BoundingSphere(float x, float y, float z, float ex, float ey, float ez, float radius);
    ~BoundingSphere();
    bool collide(float x, float y, float z);
    bool outerCollide(float x, float y, float z);
    void update(float x, float y, float z);
    Vector projectCollisionOnSurface(const Vector &position);
    Vector getCollisionNormal(const Vector &position);
};

extern "C" void AEMath_VectorAdd(void *out, const void *lhs, const void *rhs);
extern "C" void AEMath_VectorSub(void *out, const void *lhs, const void *rhs);
extern "C" void AEMath_VectorScale(void *out, float scale, const void *value);
extern "C" float AEMath_VectorDot(const void *lhs, const void *rhs);
extern "C" void AEMath_VectorNormalize(void *out, const void *value);
extern "C" float AEMath_VectorLength(const void *value);
extern "C" void *AEMath_VectorAssign(void *out, const void *value);

#endif
