#ifndef WORK_CLASSES_PLAYERSTATION_SRC_CLASS_H
#define WORK_CLASSES_PLAYERSTATION_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
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

struct Station;
struct AEGeometry;
struct BoundingVolume;
struct Array;
struct FileRead;
struct PaintCanvas;
struct Transform;

struct PlayerStaticFar {
    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStaticFar() noexcept(false);
};

struct PlayerStation : PlayerStaticFar {
    PlayerStation(Station *station);
    ~PlayerStation() noexcept(false);

    void update(int delta);
    void render();
    void setPosition(const Vector &position);
    Vector getPosition();
    void *getRoot();
    void setVisible(bool visible);
    void translate(float x, float y, float z);
    bool collide(float x, float y, float z);
    bool outerCollide(float x, float y, float z);
    void outerCollide(const Vector &position);
    Vector getProjectionVector(const Vector &position);
    Vector projectCollisionOnSurface(const Vector &position);
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

static inline void *&P(void *self, uint32_t off)
{
    return *(void **)((char *)self + off);
}

extern "C" void *operator_new(uint32_t size);
extern "C" void operator_delete(void *ptr) __attribute__((nothrow));

#endif
