#include "class.h"

// AbyssEngine::AEMath::Matrix::operator*=(Matrix const&)
// In-place affine 3x4 matrix multiply: this = this * rhs. Rows of 4 floats; the 4th
// column (offsets 0xc/0x1c/0x2c) is translation and accumulates onto its existing value.
// Finishes by multiplying the embedded Vector at +0x30.

namespace AbyssEngine {
namespace AEMath {

// Vector::operator*=(Vector const&)
extern "C" Vector *_ZN11AbyssEngine6AEMath6VectormLERKS1_(Vector *self, const Vector *other);

Matrix &Matrix::operator*=(const Matrix &p)
{
    float *t = this->m;
    const float *b = p.m;

    // Row 0
    float a0 = t[0], a1 = t[1], a2 = t[2];
    t[0] = a1 * b[4] + a0 * b[0] + a2 * b[8];
    t[1] = a1 * b[5] + a0 * b[1] + a2 * b[9];
    t[2] = a1 * b[6] + a0 * b[2] + a2 * b[10];
    t[3] = t[3] + a1 * b[7] + a0 * b[3] + a2 * b[11];

    // Row 1
    float c0 = t[4], c1 = t[5], c2 = t[6];
    t[4] = c0 * b[0] + b[4] * c1 + b[8] * c2;
    t[5] = c0 * b[1] + b[5] * c1 + b[9] * c2;
    t[6] = c0 * b[2] + b[6] * c1 + b[10] * c2;
    t[7] = t[7] + c0 * b[3] + b[7] * c1 + b[11] * c2;

    // Row 2
    float e1 = t[9], e0 = t[8], e2 = t[10];
    t[8]  = e1 * b[4] + b[0] * e0 + b[8] * e2;
    t[9]  = e1 * b[5] + b[1] * e0 + b[9] * e2;
    t[10] = e1 * b[6] + b[2] * e0 + b[10] * e2;
    t[11] = t[11] + e1 * b[7] + b[3] * e0 + b[11] * e2;

    _ZN11AbyssEngine6AEMath6VectormLERKS1_(&this->pos, &p.pos);
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
