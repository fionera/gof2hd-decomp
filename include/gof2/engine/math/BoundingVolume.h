#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H

#include <cstdint>
#include <vector>

#include "gof2/math.h"

// Galaxy on Fire 2 -- BoundingVolume class.
// A volume node carrying a center/extents pair plus an owning list of child
// volumes. Children are polled through their virtual interface for collision,
// projection and update.

class BoundingVolume {
public:
    using Vector = AbyssEngine::AEMath::Vector;

    void* vtable;
    std::vector<BoundingVolume*>* children;
    float centerX;
    float centerY;
    float centerZ;
    float extentsX;
    float extentsY;
    float extentsZ;

    BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez);
    ~BoundingVolume();

    int collide(float x, float y, float z);
    void getCollisionNormal(Vector& out);
    void setVolume(BoundingVolume* src);
    // Append this volume to the destination array. Both spellings forward to
    // the array's add.
    void setVolume_tail(std::vector<BoundingVolume*>* arr);
    void setArr(std::vector<BoundingVolume*>* arr);
    void staticProjectCollisionOnSurface(const Vector& v, std::vector<BoundingVolume*>* vols);
    void update(float x, float y, float z);
    Vector getProjectionVector(const Vector& v);
};

#endif
