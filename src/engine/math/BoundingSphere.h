#ifndef GOF2_BOUNDINGSPHERE_H
#define GOF2_BOUNDINGSPHERE_H

#include "Vector.h"
#include "engine/math/BoundingVolume.h"

#include "engine/math/AEMath.h"

class BoundingSphere : public BoundingVolume {
public:
    AbyssEngine::AEMath::Vector field_0x20;
    AbyssEngine::AEMath::Vector field_0x2c;
    float radius;
    AbyssEngine::AEMath::Vector field_0x3c;

    BoundingSphere(float x, float y, float z, float ex, float ey, float ez, float radius);

    ~BoundingSphere();

    int collide(float x, float y, float z) override;

    int outerCollide(float x, float y, float z) override;

    void update(float x, float y, float z) override;

    AbyssEngine::AEMath::Vector projectCollisionOnSurface(const AbyssEngine::AEMath::Vector &position) override;

    AbyssEngine::AEMath::Vector getCollisionNormal(const AbyssEngine::AEMath::Vector &position);
};

#if __SIZEOF_POINTER__ == 4
static_assert(__builtin_offsetof(BoundingSphere, radius) == 0x38,
              "BoundingSphere::radius offset");
static_assert(sizeof(BoundingSphere) == 0x48, "BoundingSphere size");
#endif

#endif
