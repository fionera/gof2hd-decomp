#ifndef GOF2_EXPLOSION_H
#define GOF2_EXPLOSION_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct Explosion;
struct AEGeometry;
struct Array;
struct PaintCanvas;
struct TargetFollowCamera;





static inline char *B(void *p, int off) { return (char *)p + off; }
static inline int &I(void *p, int off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, int off) { return *(uint32_t *)((char *)p + off); }
static inline uint8_t &UC(void *p, int off) { return *(uint8_t *)((char *)p + off); }
static inline float &F(void *p, int off) { return *(float *)((char *)p + off); }
static inline void *&P(void *p, int off) { return *(void **)((char *)p + off); }
static inline uint64_t &U64(void *p, int off) { return *(uint64_t *)((char *)p + off); }

static inline AEGeometry *&G(void *p, int off) { return *(AEGeometry **)((char *)p + off); }
static inline Array *&A(void *p, int off) { return *(Array **)((char *)p + off); }

struct Explosion { void* _opaque; };  // no offset accesses observed
#endif
