#ifndef GOF2_BOUNDINGVOLUME_H
#define GOF2_BOUNDINGVOLUME_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- BoundingVolume class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Uses AbyssEngine::AEMath::Vector and Array<BoundingVolume*>.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.
//
// Field layout (this):
//   0x00 void*   vtable
//   0x04 Array<BoundingVolume*>* children
//   0x08 Vector  center  (x@0x08, y@0x0c, z@0x10)
//   0x14 Vector  extents (x@0x14, y@0x18, z@0x1c)


namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct BoundingVolume;

// Array<T> layout: length@0x0, data@0x4.




// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct BoundingVolume {
    uint64_t field_0x0;                 // +0x0
    uint32_t field_0x8;                 // +0x8
};
#endif
