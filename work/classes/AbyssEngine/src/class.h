#ifndef WORK_CLASSES_ABYSSENGINE_SRC_CLASS_H
#define WORK_CLASSES_ABYSSENGINE_SRC_CLASS_H

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

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int size_t_;

namespace AbyssEngine {

// --- Small value types -----------------------------------------------------
namespace AEMath {
struct Vector { float x, y, z; };
struct Quaternion { float x, y, z, w; };
struct Matrix { float m[16]; };
} // namespace AEMath

using AEMath::Vector;
using AEMath::Quaternion;
using AEMath::Matrix;

struct ESMatrix { float m[4][4]; };

struct String {
    uint16_t *text;
    uint32_t field_04;
    uint32_t size;
};

// --- Forward-declared engine/runtime types (opaque; used by pointer) -------
struct Engine;
struct Image;
struct Image2D;
struct ImageFont;
struct Mesh;
struct Material;
struct Camera;
struct Curve;
struct SpriteSystem;
struct Transform;
struct PaintCanvas;
struct AELoadedTexture;

} // namespace AbyssEngine

// --- Field accessors via byte offset (operate on raw pointers) -------------
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &u16(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline int16_t &s16(void *self, uint32_t off) { return *(int16_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }
static inline char *bp(void *self, uint32_t off) { return (char *)self + off; }

#endif
