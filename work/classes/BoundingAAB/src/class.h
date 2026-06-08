#ifndef WORK_CLASSES_BOUNDINGAAB_SRC_CLASS_H
#define WORK_CLASSES_BOUNDINGAAB_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;

namespace AbyssEngine {
namespace AEMath {
struct Vector {
    float x;
    float y;
    float z;
};
} // namespace AEMath
} // namespace AbyssEngine

using Vector = AbyssEngine::AEMath::Vector;

struct BoundingVolume {
    BoundingVolume(float, float, float, float, float, float);
};

struct BoundingAAB : BoundingVolume {
    BoundingAAB(float, float, float, float, float, float, float, float, float);
    int collide(float, float, float);
    int outerCollide(float, float, float);
    Vector getCollisionNormal(const Vector &);
    void update(float, float, float);
    Vector projectCollisionOnSurface(const Vector &);
};

template <class T>
static inline T &F(void *self, uint32_t off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline const T &F(const void *self, uint32_t off)
{
    return *(const T *)((const char *)self + off);
}

#endif
