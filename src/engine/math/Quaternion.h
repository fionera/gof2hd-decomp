#ifndef GOF2_QUATERNION_H
#define GOF2_QUATERNION_H

#include "mathtypes.h"

namespace AbyssEngine {
    static_assert(sizeof(Quaternion) == 0x10, "Quaternion layout");
    static_assert(__builtin_offsetof(Quaternion, x) == 0x0, "Quaternion::x offset");
    static_assert(__builtin_offsetof(Quaternion, y) == 0x4, "Quaternion::y offset");
    static_assert(__builtin_offsetof(Quaternion, z) == 0x8, "Quaternion::z offset");
    static_assert(__builtin_offsetof(Quaternion, w) == 0xc, "Quaternion::w offset");
} // namespace AbyssEngine

#endif // GOF2_QUATERNION_H
