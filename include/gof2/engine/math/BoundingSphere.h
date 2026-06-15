#ifndef GOF2_BOUNDINGSPHERE_H
#define GOF2_BOUNDINGSPHERE_H

#include "gof2/math.h"
#include "gof2/engine/math/BoundingVolume.h"

// A spherical bounding volume: a BoundingVolume whose collision shape is a
// sphere of the given radius centred at the volume's centre + extents.
class BoundingSphere : public BoundingVolume {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    float radius;

    BoundingSphere(float x, float y, float z, float ex, float ey, float ez, float radius);
    // Convenience: a sphere centred at (cx,cy,cz) with zero extents and the
    // given radius. Used when a collision volume is built directly from a
    // centre + radius pair (see Globals geometry loading).
    BoundingSphere(float cx, float cy, float cz, float radius);
    ~BoundingSphere();

    bool collide(float x, float y, float z);
    bool outerCollide(float x, float y, float z);
    void update(float x, float y, float z);
    Vector projectCollisionOnSurface(const Vector& position);
    Vector getCollisionNormal(const Vector& position);

private:
    // The sphere centre in world space (volume centre offset by extents).
    Vector worldCenter() const;
};

#endif
