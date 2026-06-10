#ifndef GOF2_MININGGAME_H
#define GOF2_MININGGAME_H
#include "gof2/common.h"
// struct derived from offset-access field map (deterministic field_0xNN naming)
struct MiningGame;
struct Hud;
struct Sprite;
struct MarqueeImage;

static inline char *B(void *p, unsigned off) { return (char *)p + off; }
static inline int &I(void *p, unsigned off) { return *(int *)((char *)p + off); }
static inline uint32_t &U(void *p, unsigned off) { return *(uint32_t *)((char *)p + off); }
static inline float &F(void *p, unsigned off) { return *(float *)((char *)p + off); }
static inline uint8_t &UC(void *p, unsigned off) { return *(uint8_t *)((char *)p + off); }
static inline void *&P(void *p, unsigned off) { return *(void **)((char *)p + off); }

struct MiningGame { void* _opaque; };  // no offset accesses observed
#endif
