#ifndef WORK_CLASSES_PARTICLESYSTEMSPRITE_SRC_CLASS_H
#define WORK_CLASSES_PARTICLESYSTEMSPRITE_SRC_CLASS_H

// Galaxy on Fire 2 -- ParticleSystemSprite (Android libgof2hdaa.so, armv7 Thumb).
// Qualified target names are top-level: "ParticleSystemSprite::..." (the class is NOT in a
// namespace; only argument types like AbyssEngine::PaintCanvas are).
// Field offsets recovered per-method from the target disassembly; accessed via byte-offset casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;

struct ParticleSystemSprite;

namespace AbyssEngine {
struct PaintCanvas;
enum BlendMode { BLEND_DUMMY };
namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { char m[60]; };
}
}

struct ParticleSettings {
    struct ParticleSet;
};

template <class T> struct Array;

// Templated byte-offset field accessor: F<int>(this, 0x54) etc.
template <class T>
static inline T &F(void *p, unsigned off) {
    return *(T *)((char *)p + off);
}

#endif
