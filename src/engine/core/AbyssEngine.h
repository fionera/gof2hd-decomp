#ifndef GOF2_ABYSSENGINE_H
#define GOF2_ABYSSENGINE_H
#include "engine/core/Array.h"
#include "AEString.h"
#include "fieldaccess.h"
#include "aetypes.h"
#include "mathtypes.h"   // AbyssEngine::AEMath::Vector / Matrix, AbyssEngine::Quaternion

// Galaxy on Fire 2 -- the AbyssEngine namespace: a family of namespace-scope free functions and
// operators that build/draw/release the engine's resource types (Mesh, Image, ImageFont,
// SpriteSystem, Camera, Curve, ...) and a handful of small math/string helpers. There is no
// `class AbyssEngine`; these are standalone functions sharing the namespace.

// --- Engine/runtime types referenced by pointer -----------------------------------------------
// Declared at global scope (their full definitions live in their own render/file headers) and
// re-exported into namespace AbyssEngine below, so AbyssEngine::Engine and ::Engine name the same
// type and the extern engine prototypes match the namespace-scope call sites.
// Engine's real type lives in namespace AbyssEngine; the global alias keeps bare `Engine` working.
namespace AbyssEngine { class Engine; }
using ::AbyssEngine::Engine;
// These engine types' real types live in namespace AbyssEngine (to match the binary's mangling);
// a global `using` alias below lets bare `X` resolve to AbyssEngine::X.
namespace AbyssEngine { class Material; }
using ::AbyssEngine::Material;
class PaintCanvas;
namespace AbyssEngine { struct Image; }
using ::AbyssEngine::Image;
namespace AbyssEngine { struct Image2D; }
using ::AbyssEngine::Image2D;
namespace AbyssEngine { struct ImageFont; }
using ::AbyssEngine::ImageFont;
namespace AbyssEngine { struct Curve; }
using ::AbyssEngine::Curve;
namespace AbyssEngine { struct SpriteSystem; }
using ::AbyssEngine::SpriteSystem;
namespace AbyssEngine { struct AELoadedTexture; }
using ::AbyssEngine::AELoadedTexture;

namespace AbyssEngine {

using AEMath::Vector;
using AEMath::Matrix;
// Quaternion already lives directly in namespace AbyssEngine (see gof2/math.h).

// A bare 4x4 float matrix used by esMatrixMultiply (no separate header of its own).
struct ESMatrix { float m[4][4]; };

// Camera is a namespace-scope engine type (AbyssEngine::Camera), forward-declared here and only
// ever touched by pointer in this TU.
class Camera;

// Re-export the global engine/runtime types into this namespace. Mesh, Transform and Camera are
// intentionally NOT re-exported: their namespace-scope forms (gof2/engine/render/Mesh.h,
// gof2/engine/math/Transform.h) are distinct from any global type, so a `using ::X;` would clash.
// Engine, Image, Image2D, ImageFont, Material, Curve, SpriteSystem and AELoadedTexture already
// live in this namespace (AbyssEngine::X); no re-export needed.
using ::PaintCanvas;

} // namespace AbyssEngine

// Byte-offset accessor used by the resource helpers to reach fields of the opaque engine types
// above (which are touched only by pointer here, without their full layouts).
static inline int16_t &s16(void *self, uint32_t off) { return *(int16_t *)((char *)self + off); }

#endif // GOF2_ABYSSENGINE_H
