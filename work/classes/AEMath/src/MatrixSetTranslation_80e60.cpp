#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

extern "C" __attribute__((disable_tail_calls)) void MatrixSetTranslation(
    Matrix *result,
    Matrix *matrix,
    const Vector *translation)
{
    matrix->m[3] = translation->x;
    matrix->m[7] = translation->y;
    matrix->m[11] = translation->z;
    __aeabi_memcpy(result, matrix, sizeof(*result));
}
