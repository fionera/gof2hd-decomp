#include "class.h"

// AbyssEngine::AEMath::Matrix::operator=(Matrix const&)
// Copies the 48-byte (12-float) affine part, then delegates the embedded Vector at +0x30.

namespace AbyssEngine {
namespace AEMath {

// Vector::operator=(Vector const&)
extern "C" Vector *_ZN11AbyssEngine6AEMath6VectoraSERKS1_(Vector *self, const Vector *other);

Matrix &Matrix::operator=(const Matrix &other)
{
    for (int i = 0; i < 12; ++i)
        this->m[i] = other.m[i];
    _ZN11AbyssEngine6AEMath6VectoraSERKS1_(&this->pos, &other.pos);
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
