// @portable-fields
#ifndef WORK_CLASSES_MININGGAME_SRC_CLASS_H
#define WORK_CLASSES_MININGGAME_SRC_CLASS_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

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

#endif
