#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

extern "C" __attribute__((disable_tail_calls)) void MatrixSetRotation(
    Matrix *result,
    Matrix *matrix,
    const Vector *right,
    const Vector *up,
    const Vector *dir)
{
    matrix->m[0] = right->x;
    matrix->m[4] = right->y;
    matrix->m[8] = right->z;
    matrix->m[1] = up->x;
    matrix->m[5] = up->y;
    matrix->m[9] = up->z;
    matrix->m[2] = dir->x;
    matrix->m[6] = dir->y;
    matrix->m[10] = dir->z;
    __aeabi_memcpy(result, matrix, sizeof(*matrix));
}
