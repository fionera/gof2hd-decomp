#include "BSphere.h"


extern "C" float sqrtf(float);

// ---- Merge_74000.cpp ----
// AbyssEngine::AEMath::BSphere::Merge(BSphere const&)
// Grows *this to also enclose `other`. Handles the empty-source / empty-dest cases,
// full-containment in either direction, and otherwise expands the sphere to the minimal
// one covering both. Also tracks the looser maxRadius bound at +0x10.

namespace AbyssEngine {
namespace AEMath {

// sqrtf fallback the original calls when the inline vsqrt result is NaN.

void BSphere::Merge(const BSphere &other)
{
    if (this->maxRadius < other.maxRadius)
        this->maxRadius = other.maxRadius;

    if (other.radius == 0.0f)
        return;

    if (this->radius == 0.0f) {
        // Dest empty: copy source wholesale.
        this->radius = other.radius;
        this->cx = other.cx;
        this->cy = other.cy;
        this->cz = other.cz;
        this->maxRadius = other.maxRadius;
        *this = other;
        return;
    }

    float dx = other.cx - this->cx;
    float dy = other.cy - this->cy;
    float dz = other.cz - this->cz;
    float d2 = dy * dy + dx * dx + dz * dz;
    float dist = sqrtf(d2);

    float r2 = other.radius;
    float r1 = this->radius;

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
        this->radius = r2;
        this->cz = other.cz;
        return;
    }

    // Partial overlap: shift center along the axis and grow to cover both.
    float t = (((dist + r2) - r1) * 0.5f) / dist;
    this->cx = this->cx + dx * t;
    float newR = (dist + r1 + r2) * 0.5f;
    this->cy = this->cy + dy * t;
    this->cz = this->cz + dz * t;
    this->radius = newR;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- operator__74144.cpp ----
// AbyssEngine::AEMath::BSphere::operator=(BSphere const&)
// Plain 20-byte copy (center + both radii).

namespace AbyssEngine {
namespace AEMath {

BSphere &BSphere::operator=(const BSphere &other)
{
    this->cx = other.cx;
    this->cy = other.cy;
    this->cz = other.cz;
    this->radius = other.radius;
    this->maxRadius = other.maxRadius;
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine

// ---- Merge_73f30.cpp ----
// AbyssEngine::AEMath::BSphere::Merge(Transform const&)
// Transforms the given Transform's local bounding sphere into world space and merges it
// into *this. The world center comes from MatrixTransformVector; the world radius is the
// transform's stored radius (+0xe0) scaled by the largest absolute rotated-axis component.

namespace AbyssEngine {

namespace AEMath { struct Matrix; struct Vector; }

// extern math helpers (operate on a 3x4 matrix that here doubles as a 3-vector temp).
extern "C" void _Z21MatrixTransformVectorRN11AbyssEngine6AEMath6MatrixERKNS0_6VectorE(
    AEMath::Matrix *out, const AEMath::Vector *v);
extern "C" void _Z18MatrixRotateVectorRN11AbyssEngine6AEMath6MatrixERKNS0_6VectorE(
    AEMath::Matrix *out, const AEMath::Vector *v);

namespace AEMath {

// In-memory shape of the scratch BSphere the original builds on the stack.
struct ScratchSphere { float x, y, z, r, maxr; };

void BSphere::Merge(const Transform &t)
{
    const Vector *tv = (const Vector *)&t;
    ScratchSphere s;
    s.maxr = 1.0f;

    // World-space center = transform * local origin.
    float xfm[3];
    _Z21MatrixTransformVectorRN11AbyssEngine6AEMath6MatrixERKNS0_6VectorE(
        (Matrix *)xfm, tv);
    s.x = xfm[0];
    s.y = xfm[1];
    s.z = xfm[2];

    // Rotate the per-axis radius (stored at +0xe0) to find world extent.
    float r = *(const float *)((const char *)&t + 0xe0);
    float in[3] = { r, r, r };
    float rot[3];
    _Z18MatrixRotateVectorRN11AbyssEngine6AEMath6MatrixERKNS0_6VectorE(
        (Matrix *)rot, (const Vector *)in);

    float ax = rot[0] > 0.0f ? rot[0] : -rot[0];
    float ay = rot[1] > 0.0f ? rot[1] : -rot[1];
    float az = rot[2] > 0.0f ? rot[2] : -rot[2];
    if (ax > ay) ay = ax;
    if (ay > az) az = ay;
    s.r = az;

    Merge(*(const BSphere *)&s);
}

} // namespace AEMath
} // namespace AbyssEngine
