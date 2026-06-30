#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H

#include "Vector.h"
#include "engine/core/Array.h"

#include "engine/math/AEMath.h"

class BoundingVolume {
public:
    Array<BoundingVolume *> *children;
    float centerX;
    float centerY;
    float centerZ;
    float extentsX;
    float extentsY;
    float extentsZ;

    BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez);

    ~BoundingVolume();

    // Virtual methods in EXACT original vtable order (via _ZTV BoundingAAB ABS32 relocs): getCollisionNormal
    // @#0, update@#4, collide@#8, outerCollide@#12, projectCollisionOnSurface@#16. NO virtual destructor.
    virtual AbyssEngine::AEMath::Vector getCollisionNormal(const AbyssEngine::AEMath::Vector &position);

    virtual void update(float x, float y, float z);

    virtual int collide(float x, float y, float z);

    virtual int outerCollide(float x, float y, float z);

    virtual AbyssEngine::AEMath::Vector projectCollisionOnSurface(const AbyssEngine::AEMath::Vector &point) = 0;

    void setVolume(BoundingVolume *src);

    void setVolumes(Array<BoundingVolume *> *arr);

    void staticProjectCollisionOnSurface(const AbyssEngine::AEMath::Vector &v, Array<BoundingVolume *> *vols);

    AbyssEngine::AEMath::Vector getProjectionVector(const AbyssEngine::AEMath::Vector &v);
};

#endif
