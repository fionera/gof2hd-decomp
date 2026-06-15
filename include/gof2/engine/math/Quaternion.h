#ifndef GOF2_QUATERNION_H
#define GOF2_QUATERNION_H

// AbyssEngine::Quaternion and the AEMath::Vector / AEMath::Matrix types it operates on are
// defined together in the shared math header (they form one tightly coupled value-type cluster
// with a single canonical layout). This header re-exports that definition and verifies the
// recovered memory layout (x@0, y@4, z@8, w@12; 16 bytes).
#include "gof2/math.h"

namespace AbyssEngine {

static_assert(sizeof(Quaternion) == 0x10, "Quaternion layout");
static_assert(__builtin_offsetof(Quaternion, x) == 0x0, "Quaternion::x offset");
static_assert(__builtin_offsetof(Quaternion, y) == 0x4, "Quaternion::y offset");
static_assert(__builtin_offsetof(Quaternion, z) == 0x8, "Quaternion::z offset");
static_assert(__builtin_offsetof(Quaternion, w) == 0xc, "Quaternion::w offset");

} // namespace AbyssEngine

#endif // GOF2_QUATERNION_H
