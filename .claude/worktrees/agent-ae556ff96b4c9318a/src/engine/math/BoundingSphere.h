#ifndef GOF2_BOUNDINGSPHERE_H
#define GOF2_BOUNDINGSPHERE_H

#include "mathtypes.h"
#include "engine/math/BoundingVolume.h"

// A spherical bounding volume: a BoundingVolume whose collision shape is a
// sphere of the given radius centred at the volume's centre + extents.
class BoundingSphere : public BoundingVolume {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    float radius;

    BoundingSphere(float x, float y, float z, float ex, float ey, float ez, float radius);
    ~BoundingSphere();

    int collide(float x, float y, float z) override;
    int outerCollide(float x, float y, float z) override;
    void update(float x, float y, float z) override;
    Vector projectCollisionOnSurface(const Vector& position) override;
    Vector getCollisionNormal(const Vector& position);
};

#endif
