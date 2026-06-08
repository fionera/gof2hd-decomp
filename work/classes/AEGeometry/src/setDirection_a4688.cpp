#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::setDirection(const Vector& dir, const Vector& up)
//   Builds an orthonormal orientation from the requested forward direction and
//   up hint (Gram-Schmidt), writes it as the transform's local matrix while
//   preserving the existing translation, then re-applies the stored scaling.
extern "C" void VectorCross(Vector *out, const Vector *b);                 // 0x6ec8c (out = out x b)
extern "C" void _ae_MatrixSetScaling(void *out, uint32_t loc, float sx, float sy, float sz);
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n); // 0x6f1e4

struct AEGeometry { void setDirection(const Vector &dir, const Vector &up); };

void AEGeometry::setDirection(const Vector &dir, const Vector &up)
{
    char local[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    __aeabi_memcpy_b(local, (void *)loc, 0x3c);

    // right = normalize(up x dir)
    Vector right = up;
    VectorCross(&right, &dir);
    Vector tmp;
    VectorNormalize(&tmp, &right);
    right = tmp;

    // recomputedUp = normalize(right x dir)
    Vector rUp;
    VectorCross(&tmp, &dir);     // tmp currently == normalized right
    rUp = tmp;
    VectorNormalize(&tmp, &rUp);
    rUp = tmp;

    // Assemble basis rows (right, up, forward) into the local matrix.
    Matrix &m = *(Matrix *)local;
    m.m[0] = right.x;  m.m[1] = right.y;  m.m[2] = right.z;
    m.m[3] = rUp.x;    m.m[4] = rUp.y;    m.m[5] = rUp.z;
    m.m[6] = dir.x;    m.m[7] = dir.y;    m.m[8] = dir.z;

    PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)this->f_2c,
                                   this->f_c, &m);
    loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    _ae_MatrixSetScaling((void *)local, loc, this->f_3c, this->f_40, this->f_44);
}
