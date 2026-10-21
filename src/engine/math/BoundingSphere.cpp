#include "engine/math/BoundingSphere.h"
#include "engine/math/AEMath.h"

BoundingSphere::BoundingSphere(
    float x, float y, float z, float ex, float ey, float ez, float radius)
    : BoundingVolume(x, y, z, ex, ey, ez) {
    this->radius = radius;
}

BoundingSphere::~BoundingSphere() {
}

void BoundingSphere::update(float x, float y, float z) {
    BoundingVolume::update(x, y, z);
}

Vector BoundingSphere::projectCollisionOnSurface(const Vector &point) {
    Vector delta = (position + offset) - point;
    float length = VectorLength(delta);
    float radius = this->radius;
    if (length >= radius)
        return Vector{0.0f, 0.0f, 0.0f};
    return (position + offset) - (radius / length) * delta;
}

int BoundingSphere::outerCollide(float x, float y, float z) {
    Vector delta = Vector{x, y, z} - (position + offset);
    return VectorDot(delta, delta) < radius * radius ? 1 : 0;
}

Vector BoundingSphere::getCollisionNormal(const Vector &point) {
    Vector normal = (position + offset) - point;
    normal = VectorNormalize(normal);
    return normal;
}

int BoundingSphere::collide(float x, float y, float z) {
    if (outerCollide(x, y, z)) {
        return 1;
    }
    return BoundingVolume::collide(x, y, z);
}
