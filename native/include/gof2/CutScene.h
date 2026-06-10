#ifndef GOF2_CUTSCENE_H
#define GOF2_CUTSCENE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- CutScene class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Field offsets recovered per-method from the target
// disassembly; accessed via byte-offset casts from `this`.


struct AEGeometry;
struct Level;



// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct CutScene { void* _opaque; };  // no offset accesses observed
#endif
