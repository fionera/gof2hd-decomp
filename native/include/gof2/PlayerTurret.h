#ifndef GOF2_PLAYERTURRET_H
#define GOF2_PLAYERTURRET_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
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

void *operator new(__SIZE_TYPE__ size);
void operator delete(void *ptr) noexcept;

extern "C" void *__aeabi_memcpy(void *dst, const void *src, uint32_t n);

struct PlayerTurret { void* _opaque; };  // no offset accesses observed
#endif
