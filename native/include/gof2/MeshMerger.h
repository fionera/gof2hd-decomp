#ifndef GOF2_MESHMERGER_H
#define GOF2_MESHMERGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- MeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.


namespace AbyssEngine {

struct Mesh;
struct PaintCanvas;

namespace AEMath {

// AEMath::Matrix is a 15-float (0x3c byte) object with an engine ctor/operator=.

} // namespace AEMath

} // namespace AbyssEngine

using Mesh = AbyssEngine::Mesh;
using PaintCanvas = AbyssEngine::PaintCanvas;
using Matrix = AbyssEngine::AEMath::Matrix;
using Vector = AbyssEngine::AEMath::Vector;



// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline int16_t &i16(void *self, uint32_t off) { return *(int16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline int8_t &i8(void *self, uint32_t off) { return *(int8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

struct MeshMerger { void* _opaque; };  // no offset accesses observed
#endif
