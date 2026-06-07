#ifndef WORK_CLASSES_PLAYERSTATIC_SRC_CLASS_H
#define WORK_CLASSES_PLAYERSTATIC_SRC_CLASS_H

// Galaxy on Fire 2 -- PlayerStatic class (Android libgof2hdaa.so, armv7 Thumb).
// Target names are top-level PlayerStatic::... (no AbyssEngine:: on the class).
// Field offsets come from the per-method work-items and are accessed by byte casts.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
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

struct AEGeometry;

struct PlayerStatic {
    PlayerStatic(int playerId, AEGeometry *geometry, float x, float y, float z);
    ~PlayerStatic();

    Vector getPosition();
    void render();
};

static inline int32_t &i32(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &u32(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline float &f32(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&pp(void *self, uint32_t off) { return *(void **)((char *)self + off); }

#endif
