#ifndef GOF2_BOUNDINGAAB_H
#define GOF2_BOUNDINGAAB_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "engine/math/BoundingVolume.h"
// Galaxy on Fire 2 -- BoundingAAB (axis-aligned box) bounding volume.
// Derives from BoundingVolume; adds per-axis half-extents on top of the
// inherited center/extents.

class BoundingAAB : public BoundingVolume {
public:
    float halfExtentX;
    float halfExtentY;
    float halfExtentZ;

    BoundingAAB(float x, float y, float z, float ex, float ey, float ez,
                float width, float height, float depth);
    int outerCollide(float x, float y, float z);
    Vector projectCollisionOnSurface(const Vector &point);
    Vector getCollisionNormal(const Vector &point);
    int collide(float x, float y, float z);
    void update(float x, float y, float z);
};
#endif
