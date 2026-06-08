#include "class.h"

// AbyssEngine::AEMath::BSphere::Merge(BSphere const&)
// Grows *this to also enclose `other`. Handles the empty-source / empty-dest cases,
// full-containment in either direction, and otherwise expands the sphere to the minimal
// one covering both. Also tracks the looser maxRadius bound at +0x10.

namespace AbyssEngine {
namespace AEMath {

// sqrtf fallback the original calls when the inline vsqrt result is NaN.
extern "C" float sqrtf(float);

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
