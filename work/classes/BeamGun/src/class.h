#ifndef WORK_CLASSES_BEAMGUN_SRC_CLASS_H
#define WORK_CLASSES_BEAMGUN_SRC_CLASS_H

// Galaxy on Fire 2 -- BeamGun (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level "BeamGun::..." symbols. Field offsets are recovered
// per method and accessed directly instead of modeling a full class layout.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct BeamGun;
struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct AEGeometry;
struct PaintCanvas;
struct Transform;
struct Array;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x, y, z;
};

struct Matrix {
    float m[15];
};

} // namespace AEMath
} // namespace AbyssEngine

typedef AbyssEngine::AEMath::Vector Vector;
typedef AbyssEngine::AEMath::Matrix Matrix;

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &F(const void *self, unsigned off)
{
    return *(const T *)((const char *)self + off);
}

#endif
