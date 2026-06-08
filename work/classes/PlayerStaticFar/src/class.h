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
    // @portable-fields
    void* f_0; // 0x0
    void* f_4; // 0x4
    void* f_8; // 0x8
    unsigned char _pad_c[76];
    int f_58; // 0x58
    int f_5c; // 0x5c
    int f_60; // 0x60
    unsigned char _pad_64[192];
    int f_124; // 0x124
    int f_128; // 0x128
    int f_12c; // 0x12c
    void* f_130; // 0x130
    int f_134; // 0x134
    int f_138; // 0x138
    int f_13c; // 0x13c

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
