#ifndef GOF2_QUATERNION_H
#define GOF2_QUATERNION_H
#include "gof2/common.h"
// real struct kept from byte-match recovery (+ supporting decls)
extern "C" float sqrtf(float);
extern "C" float sinf(float);
extern "C" float cosf(float);

namespace AbyssEngine {

namespace AEMath {





} // namespace AEMath



static_assert(sizeof(AEMath::Vector) == 0xc, "AEMath::Vector layout");
static_assert(sizeof(AEMath::Matrix) == 0x40, "AEMath::Matrix layout");
static_assert(sizeof(Quaternion) == 0x10, "Quaternion layout");
static_assert(__builtin_offsetof(Quaternion, x) == 0x0, "Quaternion::x offset");
static_assert(__builtin_offsetof(Quaternion, y) == 0x4, "Quaternion::y offset");
static_assert(__builtin_offsetof(Quaternion, z) == 0x8, "Quaternion::z offset");
static_assert(__builtin_offsetof(Quaternion, w) == 0xc, "Quaternion::w offset");

} // namespace AbyssEngine
#endif
