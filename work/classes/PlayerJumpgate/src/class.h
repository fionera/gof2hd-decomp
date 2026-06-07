#ifndef WORK_CLASSES_PLAYERJUMPGATE_SRC_CLASS_H
#define WORK_CLASSES_PLAYERJUMPGATE_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef int int32_t;
typedef long long int64_t;

struct AEGeometry;
struct BoundingVolume;
struct BoundingSphere;
struct PlayerJumpgate;
struct PlayerStaticFar;

template <class T>
struct Array {
    uint32_t length;
    T *data;
    uint32_t capacity;
};

struct PlayerJumpgate {
    PlayerJumpgate(int playerId, AEGeometry *geometry, float x, float y, float z, bool visible);
    ~PlayerJumpgate() noexcept(false);

    void addJumpAnimationHandle(uint32_t handle);
    void activate();
    bool timeToJump();
    bool animationEnded();
    void setPosition(float x, float y, float z);
    void update(int delta);
};

template <class T>
static inline T &F(void *self, unsigned off)
{
    return *(T *)((char *)self + off);
}

template <class T>
static inline volatile T &VF(void *self, unsigned off)
{
    return *(volatile T *)((char *)self + off);
}

#endif
