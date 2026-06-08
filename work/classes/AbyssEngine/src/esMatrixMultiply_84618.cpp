#include "class.h"

extern "C" void *__aeabi_memcpy(void *dst, const void *src, size_t_ n);

// AbyssEngine::esMatrixMultiply(ESMatrix* out, ESMatrix* a, ESMatrix* b)
// Standard row-major 4x4 multiply: out = a * b. The shipped code uses NEON
// multiply/multiply-accumulate over the columns of b weighted by each row of a, then
// memcpy's the scratch result over `out` (so out may alias a or b safely).
namespace AbyssEngine {

void esMatrixMultiply(ESMatrix *out, ESMatrix *a, ESMatrix *b)
{
    float local[4][4];
    const float (*A)[4] = a->m;
    const float (*B)[4] = b->m;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            local[i][j] = A[i][0] * B[0][j] + A[i][1] * B[1][j] + A[i][2] * B[2][j] +
                          A[i][3] * B[3][j];
        }
    }

    __aeabi_memcpy(out, local, 0x40);
}

} // namespace AbyssEngine
