#include "class.h"

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

extern "C" __attribute__((stack_protect_req, disable_tail_calls)) void MatrixSetScaling(
    Matrix *result,
    Matrix *matrix,
    float x,
    float y,
    float z)
{
    matrix->m[0] *= x;
    matrix->m[4] *= x;
    matrix->m[8] *= x;
    matrix->m[1] *= y;
    matrix->m[5] *= y;
    matrix->m[9] *= y;
    matrix->m[2] *= z;
    matrix->m[6] *= z;
    matrix->m[10] *= z;

    Vector scale;
    scale.x = x;
    scale.y = y;
    scale.z = z;
    *reinterpret_cast<Vector *>(&matrix->m[12]) = scale;

    __aeabi_memcpy(result, matrix, sizeof(*matrix));
}
