#include "class.h"

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
