#include "class.h"
#include <arm_neon.h>

namespace AbyssEngine {
namespace AEMath {

BSphere &BSphere::operator=(const BSphere &other) {
    const unsigned int *src = reinterpret_cast<const unsigned int *>(&other);
    unsigned int *dst = reinterpret_cast<unsigned int *>(this);

    uint32x4_t first = vld1q_u32(src);
    vst1q_u32(dst, first);
    dst[4] = src[4];
    return *this;
}

} // namespace AEMath
} // namespace AbyssEngine
