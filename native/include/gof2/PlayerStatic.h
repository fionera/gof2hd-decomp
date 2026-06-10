#ifndef GOF2_PLAYERSTATIC_H
#define GOF2_PLAYERSTATIC_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- PlayerStatic class (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level PlayerStatic::... (no AbyssEngine:: on the class).
// Field offsets come from the per-method work-items and are accessed by byte casts.


namespace AbyssEngine {
namespace AEMath {

} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct AEGeometry;



static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct PlayerStatic { void* _opaque; };  // no offset accesses observed
#endif
