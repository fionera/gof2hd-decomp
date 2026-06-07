#ifndef WORK_CLASSES_PLAYERSTATICFAR_SRC_CLASS_H
#define WORK_CLASSES_PLAYERSTATICFAR_SRC_CLASS_H

// Galaxy on Fire 2 -- PlayerStaticFar class (Android libgof2hdaa.so, armv7 Thumb).
// Top-level class (no AbyssEngine:: on the class itself) but its API uses
// AbyssEngine::AEMath::Vector. Field offsets recovered per-method from the
// target disassembly; accessed via byte-offset casts from `this`.

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

struct AEGeometry;

struct PlayerStaticFar {
    PlayerStaticFar(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStaticFar() noexcept(false);

    void update(int delta);
    void render();
    bool collide(float x, float y, float z);
    bool outerCollide(float x, float y, float z);
    void outerCollide(Vector value);
    Vector getInitPosition(Vector out);
    Vector getProjectionVector(const Vector &value);
    Vector projectCollisionOnSurface(const Vector &value);
};

// Field accessors via byte offset.
static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &u8(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
