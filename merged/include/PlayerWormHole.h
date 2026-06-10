#ifndef WORK_CLASSES_PLAYERWORMHOLE_SRC_CLASS_H
#define WORK_CLASSES_PLAYERWORMHOLE_SRC_CLASS_H

// Galaxy on Fire 2 -- PlayerWormHole (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level PlayerWormHole::... (no AbyssEngine namespace).
// Field offsets come from the per-method work-items and are accessed by byte casts.

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct PlayerWormHole;
struct AEGeometry;

namespace AbyssEngine {
struct String;

namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    char m[60];
};
}
}

using Vector = AbyssEngine::AEMath::Vector;
using Matrix = AbyssEngine::AEMath::Matrix;

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

#endif
