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
class BoundingSphere : public BoundingVolume  {
public:
    // BoundingVolume base occupies 0x00..0x1f (size 0x20). The compiler-laid
    // base is followed by sphere-specific fields. The real binary places the
    // radius at object offset 0x38; padding members keep the layout faithful.
    char field_0x20[0x18];   // padding to reach radius at 0x38
    float field_0x38;        // +0x38 radius

    BoundingSphere(float x, float y, float z, float ex, float ey, float ez, float radius);
    // Convenience: a sphere centred at (cx,cy,cz) with zero extents and the
    // given radius. Used when a collision volume is built directly from a
    // centre + radius pair (see Globals geometry loading).
    BoundingSphere(float cx, float cy, float cz, float radius);
    ~BoundingSphere();
    bool collide(float x, float y, float z);
    bool outerCollide(float x, float y, float z);
    void update(float x, float y, float z);
    Vector projectCollisionOnSurface(const Vector &position);
    Vector getCollisionNormal(const Vector &position);
};

// Real AbyssEngine::AEMath vector ops (defined in src/AEMath.cpp). Declared here
// directly (rather than including gof2/AEMath.h) to avoid pulling in AEMath.h's
// BSphere/Transform definitions, which clash with gof2/Transform.h in some TUs.
namespace AbyssEngine { namespace AEMath {
float VectorDot(const Vector &lhs, const Vector &rhs);
Vector VectorNormalize(const Vector &value);
float VectorLength(const Vector &value);
Vector operator+(const Vector &lhs, const Vector &rhs);
Vector operator-(const Vector &lhs, const Vector &rhs);
Vector operator*(const Vector &lhs, float rhs);
} }

// Thin typed wrappers over the real AbyssEngine::AEMath vector ops.
inline void AEMath_VectorAdd(void *out, const void *lhs, const void *rhs) {
    *(Vector *)out = *(const Vector *)lhs + *(const Vector *)rhs;
}
inline void AEMath_VectorSub(void *out, const void *lhs, const void *rhs) {
    *(Vector *)out = *(const Vector *)lhs - *(const Vector *)rhs;
}
inline void AEMath_VectorScale(void *out, float scale, const void *value) {
    *(Vector *)out = *(const Vector *)value * scale;
}
inline float AEMath_VectorDot(const void *lhs, const void *rhs) {
    return AbyssEngine::AEMath::VectorDot(*(const Vector *)lhs, *(const Vector *)rhs);
}
inline void AEMath_VectorNormalize(void *out, const void *value) {
    *(Vector *)out = AbyssEngine::AEMath::VectorNormalize(*(const Vector *)value);
}
inline float AEMath_VectorLength(const void *value) {
    return AbyssEngine::AEMath::VectorLength(*(const Vector *)value);
}
inline void *AEMath_VectorAssign(void *out, const void *value) {
    *(Vector *)out = *(const Vector *)value;
    return out;
}

#endif
