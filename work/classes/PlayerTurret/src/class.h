// @portable-fields
#ifndef WORK_CLASSES_PLAYERTURRET_SRC_CLASS_H
#define WORK_CLASSES_PLAYERTURRET_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

struct PlayerTurret;
struct Player;
struct KIPlayer;
struct Level;
struct AEGeometry;
struct Explosion;
struct Standing;
struct TargetFollowCamera;

namespace AbyssEngine {
namespace AEMath {

struct Vector {
    float x;
    float y;
    float z;
};

struct Matrix {
    float m[15];
};

} // namespace AEMath
} // namespace AbyssEngine

struct IntArray {
    uint32_t length;
    int *data;
    uint32_t capacity;
};

struct PlayerArray {
    uint32_t length;
    Player **data;
    uint32_t capacity;
};

static inline char *B(void *self, uint32_t off) { return (char *)self + off; }
static inline const char *B(const void *self, uint32_t off) { return (const char *)self + off; }
static inline int32_t &I(void *self, uint32_t off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &U(void *self, uint32_t off) { return *(uint32_t *)((char *)self + off); }
static inline uint16_t &US(void *self, uint32_t off) { return *(uint16_t *)((char *)self + off); }
static inline uint8_t &UC(void *self, uint32_t off) { return *(uint8_t *)((char *)self + off); }
static inline float &F(void *self, uint32_t off) { return *(float *)((char *)self + off); }
static inline void *&P(void *self, uint32_t off) { return *(void **)((char *)self + off); }

template <class T>
static inline T *TP(void *self, uint32_t off)
{
    return *(T **)((char *)self + off);
}

void *operator new(uint32_t size);
void operator delete(void *ptr) noexcept;

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

#endif
