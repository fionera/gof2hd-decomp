#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {
namespace AEMath {

namespace {
struct MatrixTail {
    unsigned long long data[2];
} __attribute__((packed, aligned(4)));

const MatrixTail kMatrixTail __attribute__((section(".text"))) = {
    {0x000000003f800000ULL, 0}
};
} // namespace

Matrix::Matrix()
{
    uint32x4_t zero = vdupq_n_u32(0);
    uint64x2_t tail_vec = vld1q_u64(
        reinterpret_cast<const unsigned long long *>(&kMatrixTail.data[0]));
    unsigned int one = 0x3f800000;
    volatile unsigned int *out = reinterpret_cast<volatile unsigned int *>(m);

    out[0] = one;
    out[5] = one;
    out[14] = one;
    vst1q_u32(reinterpret_cast<unsigned int *>(m + 1), zero);
    vst1q_u32(reinterpret_cast<unsigned int *>(m + 6), zero);
    vst1q_u32(reinterpret_cast<unsigned int *>(m + 10),
              vreinterpretq_u32_u64(tail_vec));
}

} // namespace AEMath
} // namespace AbyssEngine
