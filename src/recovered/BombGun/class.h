#ifndef WORK_CLASSES_BOMBGUN_SRC_CLASS_H
#define WORK_CLASSES_BOMBGUN_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

struct BombGun;
struct RocketGun;
struct Gun;
struct Level;
struct Player;
struct PlayerEgo;
struct LevelScript;
struct Explosion;
struct AEGeometry;
struct PaintCanvas;
struct Transform;
struct TargetFollowCamera;
struct FModSound;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    char m[0x3c];
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

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

#endif
