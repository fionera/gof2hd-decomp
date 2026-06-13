#ifndef GOF2_ABYSSENGINE_H
#define GOF2_ABYSSENGINE_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
// Galaxy on Fire 2 -- AbyssEngine namespace-scope free functions and small math/string
// helpers (Android libgof2hdaa.so, armv7 Thumb).
//
// The work items tagged "class AbyssEngine" are NOT methods of a class named AbyssEngine.
// They are free functions living in `namespace AbyssEngine`. Ghidra renders struct-return
// helpers with a leading `AbyssEngine *this` argument -- that is the hidden return-slot
// pointer (sret), not a real `this`. We model those as ordinary free functions whose return
// value is the produced struct, so the symbol still demangles to AbyssEngine::name(...).
//
// All aggregate fields are reached via byte-offset casts so we never need exact struct
// layouts; engine/runtime callees are declared extern "C" by their mangled names.

typedef unsigned int size_t_;

// --- Engine/runtime types declared at GLOBAL scope ------------------------
// fwd.h already declares Engine/Mesh/Material/Camera/Transform/PaintCanvas globally; the
// remaining engine types are declared here once at global scope. The AbyssEngine namespace
// re-exports them via `using` so AbyssEngine::Engine and ::Engine are the same type and the
// top-level extern "C" engine prototypes match the namespace-scope call sites.
struct Image;
struct Image2D;
struct ImageFont;
struct Curve;
struct SpriteSystem;
struct AELoadedTexture;

namespace AbyssEngine {

// --- Small value types -----------------------------------------------------
namespace AEMath {

} // namespace AEMath

using AEMath::Vector;
using AEMath::Matrix;
// Quaternion lives directly in namespace AbyssEngine (see gof2/math.h); already visible here.

struct ESMatrix { float m[4][4]; };

// --- Re-export the global engine/runtime types into this namespace --------
// NOTE: Mesh and Transform are intentionally NOT re-exported here. gof2/Mesh.h declares
// AbyssEngine::Mesh / AbyssEngine::Transform as namespace-scope forward decls (distinct from
// the global complete ::Mesh struct), so a `using ::Mesh;` would conflict. Code that needs the
// complete Mesh layout uses the global ::Mesh explicitly at the access site.
using ::Engine;
using ::Image;
using ::Image2D;
using ::ImageFont;
using ::Material;
using ::Camera;
using ::Curve;
using ::SpriteSystem;
using ::PaintCanvas;
using ::AELoadedTexture;

} // namespace AbyssEngine

// --- Field accessors via byte offset (operate on raw pointers) -------------
static inline int16_t &s16(void *self, uint32_t off) { return *(int16_t *)((char *)self + off); }
static inline char *bp(void *self, uint32_t off) { return (char *)self + off; }

// NOTE: AbyssEngine is a namespace (declared above), not a class. No struct here.
#endif
