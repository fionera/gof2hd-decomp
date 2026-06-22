#include "engine/math/BSphere.h"
#include "mathtypes.h"
#include <cmath>

namespace AbyssEngine {

namespace AEMath {

// Engine math helpers (defined in AEMath.cpp).
Vector MatrixTransformVector(const Matrix &matrix, const Vector &vector);
Vector MatrixRotateVector(const Matrix &matrix, const Vector &vector);

BSphere &BSphere::operator=(const BSphere &other)
{
    this->cx = other.cx;
    this->cy = other.cy;
    this->cz = other.cz;
    this->radius = other.radius;
    this->maxRadius = other.maxRadius;
    return *this;
}

// Grows *this to also enclose `other`. Handles the empty-source / empty-dest cases,
// full-containment in either direction, and otherwise expands the sphere to the
// minimal one covering both, while tracking the looser maxRadius bound.
void BSphere::Merge(const BSphere &other)
{
    if (this->maxRadius < other.maxRadius)
        this->maxRadius = other.maxRadius;

    if (other.radius == 0.0f)
        return;

    if (this->radius == 0.0f) {
        // Dest empty: copy source wholesale.
        *this = other;
        return;
    }

    float dx = other.cx - this->cx;
    float dy = other.cy - this->cy;
    float dz = other.cz - this->cz;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    float r1 = this->radius;
    float r2 = other.radius;

    if (dist == 0.0f) {
        // Concentric: keep the larger radius.
        if (r2 < r1)
            r2 = r1;
        this->radius = r2;
        return;
    }

    // Source fully inside dest -> nothing to do.
    if (r1 > dist + r2)
        return;

    // Dest fully inside source -> become source.
    if (dist - r2 < -r1) {
        this->cx = other.cx;
        this->cy = other.cy;
        this->cz = other.cz;
        this->radius = r2;
        return;
    }

    // Partial overlap: shift center along the axis and grow to cover both.
    float t = (((dist + r2) - r1) * 0.5f) / dist;
    this->cx = this->cx + dx * t;
    this->cy = this->cy + dy * t;
    this->cz = this->cz + dz * t;
    this->radius = (dist + r1 + r2) * 0.5f;
}

// Transforms `t`'s local bounding sphere into world space and merges it into *this.
// The world center comes from the transform's world matrix; the world radius is the
// transform's stored bounding radius scaled by the largest absolute rotated-axis
// component.
void BSphere::Merge(const Transform &t)
{
    // Transform layout: world matrix at +0x00, bounding radius at +0xe0. The full
    // Transform definition cannot be included here because gof2/engine/math/Transform.h
    // (and AEMath.h) still define a conflicting inline AEMath::BSphere.
    const Matrix &worldMatrix = *reinterpret_cast<const Matrix *>(&t);
    float boundingRadius = *reinterpret_cast<const float *>(
        reinterpret_cast<const char *>(&t) + 0xe0);

    Vector origin = { 0.0f, 0.0f, 0.0f };
    Vector center = MatrixTransformVector(worldMatrix, origin);

    Vector axisRadius = { boundingRadius, boundingRadius, boundingRadius };
    Vector rotated = MatrixRotateVector(worldMatrix, axisRadius);

    float ax = std::fabs(rotated.x);
    float ay = std::fabs(rotated.y);
    float az = std::fabs(rotated.z);
    if (ax > ay) ay = ax;
    if (ay > az) az = ay;

    BSphere world;
    world.cx = center.x;
    world.cy = center.y;
    world.cz = center.z;
    world.radius = az;
    world.maxRadius = 1.0f;
    Merge(world);
}

} // namespace AEMath
} // namespace AbyssEngine
