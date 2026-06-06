#include "class.h"
#include <arm_neon.h>

extern "C" unsigned int __stack_chk_guard;
extern "C" void __stack_chk_fail() __attribute__((noreturn));
extern "C" void __aeabi_memcpy(void *, const void *, unsigned long);

using AbyssEngine::AEMath::Matrix;
using AbyssEngine::AEMath::Vector;

extern "C" __attribute__((disable_tail_calls)) void MatrixIdentity(Matrix *result,
                                                                   Matrix *matrix)
{
    volatile unsigned int cookie = __stack_chk_guard;
    float32x4_t zero = vdupq_n_f32(0.0f);

    vst1q_f32(&matrix->m[1], zero);
    vst1q_f32(&matrix->m[6], zero);

    volatile unsigned int *out = reinterpret_cast<volatile unsigned int *>(matrix->m);
    unsigned int one = 0x3f800000;
    out[0] = one;
    out[5] = one;
    matrix->m[10] = 1.0f;
    matrix->m[11] = 0.0f;

    Vector diagonal;
    diagonal.x = 1.0f;
    diagonal.y = 1.0f;
    diagonal.z = 1.0f;
    *reinterpret_cast<Vector *>(&matrix->m[12]) = diagonal;

    __aeabi_memcpy(result, matrix, sizeof(*result));
    if (cookie == __stack_chk_guard) {
        return;
    }
    __stack_chk_fail();
}
