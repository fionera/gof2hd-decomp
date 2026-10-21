#include "engine/math/BoundingVolume.h"
#include "engine/math/AEMath.h"

namespace AEMath = AbyssEngine::AEMath;
using Vector = AbyssEngine::AEMath::Vector;

BoundingVolume::~BoundingVolume() {
    if (children != nullptr) {
        ArrayReleaseClasses(*children);
        delete children;
    }
    children = nullptr;
}

int BoundingVolume::collide(float, float, float) {
    if (children != nullptr) {
        for (unsigned int i = 0; i < children->size(); i++) {
            if ((*children)[i]->collide(position.x, position.y, position.z) != 0)
                return 1;
        }
    }
    return 0;
}

int BoundingVolume::outerCollide(float, float, float) {
    return 0;
}

Vector BoundingVolume::getCollisionNormal(const Vector &position) {
    (void) position;
    Vector out;
    out.x = 0.0f;
    out.y = 0.0f;
    out.z = 0.0f;
    return out;
}

void BoundingVolume::setVolume(BoundingVolume *src) {
    children = new Array<BoundingVolume *>();
    ArrayAdd(src, *children);
}

void BoundingVolume::setVolumes(Array<BoundingVolume *> *arr) {
    children = arr;
}

BoundingVolume::BoundingVolume(float cx, float cy, float cz, float ex, float ey, float ez) {
    children = nullptr;
    position.x = cx;
    position.y = cy;
    position.z = cz;
    offset.x = ex;
    offset.z = ez;
    offset.y = ey;
}

// Pushes the point out of every volume it lies in (two passes, so overlapping volumes settle).
Vector BoundingVolume::staticProjectCollisionOnSurface(const Vector &point, Array<BoundingVolume *> *volumes) {
    Vector result = point;
    if (volumes != nullptr) {
        for (int pass = 0; pass != 2; pass++) {
            for (unsigned int i = 0; i < volumes->size(); i++) {
                if ((*volumes)[i]->outerCollide(result.x, result.y, result.z) != 0)
                    result = (*volumes)[i]->projectCollisionOnSurface(result);
            }
        }
    }
    return result;
}

void BoundingVolume::update(float x, float y, float z) {
    if (children != nullptr) {
        for (unsigned int i = 0; i < children->size(); i++)
            (*children)[i]->update(x, y, z);
    }
    position = Vector{x, y, z};
}

Vector BoundingVolume::getProjectionVector(const Vector &point) {
    Vector direction = point - position;
    direction = AEMath::VectorNormalize(direction);
    return direction;
}
