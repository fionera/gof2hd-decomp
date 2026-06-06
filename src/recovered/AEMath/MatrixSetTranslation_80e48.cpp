#include "class.h"

extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

namespace AbyssEngine {
namespace AEMath {

__attribute__((disable_tail_calls))
void MatrixSetTranslation(Matrix *result, Matrix &matrix, float x, float y, float z)
{
    volatile float *values = matrix.m;
    values[7] = y;
    values[3] = x;
    values[11] = z;
    __aeabi_memcpy(result, &matrix, sizeof(matrix));
}

} // namespace AEMath
} // namespace AbyssEngine
