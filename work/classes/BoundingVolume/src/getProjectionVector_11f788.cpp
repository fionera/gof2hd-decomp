#include "class.h"

// AbyssEngine::AEMath::operator-(Vector&out, Vector const&a, Vector const&b)
extern "C" void AEMath_operator_sub(Vector *out, const Vector *a, const Vector *b); // 0x6ec38
extern "C" void AEMath_VectorNormalize(void *out, const Vector *v);                 // 0x6ec80
extern "C" void AEMath_Vector_assign(Vector *dst, const Vector *src);               // 0x6eb3c

// getProjectionVector(v): returns normalize(v - this->center).
Vector BoundingVolume::getProjectionVector(const Vector &v)
{
    Vector ret;
    AEMath_operator_sub(&ret, &v, (const Vector *)((char *)this + 8));
    char tmp[12];
    AEMath_VectorNormalize(tmp, &ret);
    AEMath_Vector_assign(&ret, (const Vector *)tmp);
    return ret;
}
