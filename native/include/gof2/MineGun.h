#ifndef GOF2_MINEGUN_H
#define GOF2_MINEGUN_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Gun;
struct Level;
struct PlayerEgo;
struct Explosion;
struct AEGeometry;
struct TargetFollowCamera;

namespace AbyssEngine {
namespace AEMath {

}
}

typedef AbyssEngine::AEMath::Vector Vector;







static inline void *&P(void *self, unsigned off) { return *(void **)((char *)self + off); }
static inline int32_t &I(void *self, unsigned off) { return *(int32_t *)((char *)self + off); }
static inline uint32_t &U(void *self, unsigned off) { return *(uint32_t *)((char *)self + off); }
static inline uint8_t &UC(void *self, unsigned off) { return *(uint8_t *)((char *)self + off); }
static inline float &FL(void *self, unsigned off) { return *(float *)((char *)self + off); }

typedef int v4i __attribute__((__vector_size__(16), __aligned__(4)));

struct MineGun { void* _opaque; };  // no offset accesses observed
#endif
