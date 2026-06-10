#ifndef GOF2_LODMESHMERGER_H
#define GOF2_LODMESHMERGER_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- LodMeshMerger class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class. Its API uses AbyssEngine::{Mesh, PaintCanvas, AEMath::Matrix}.
// Field offsets recovered per-method from the target disassembly; accessed via
// byte-offset casts from `this`.
//
// Field layout (this):
//   0x00 int   rows
//   0x04 u16   flags
//   0x06 u8    initFlag
//   0x08 Array<Mesh*> {len@0x08, data@0x0c}
//   0x14 PaintCanvas* canvas
//   0x18 u32   meshId
//   0x1c u32   transformId
//   0x20 void* outMesh
//   0x24 void* slots (transformed-mesh ptr table)
//   0x28 Matrix* matrices (0x3c each)
//   0x2c int8*  lods
//   0x30 u8*    enabled
//   0x34 u8*    visible
//   0x38 int    cols
//   0x3c u8     dirty


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

struct LodMeshMerger { void* _opaque; };  // no offset accesses observed
#endif
